/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 * 4* 7 segment, TM1637 : https://github.com/avishorp/TM1637
 * LCD Matrix 8*8 : http://wayoda.github.io/LedControl/
 * RTC DS3231 (3.3v, SDA, SCL, empty, GND). Adress: 0x68
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DirectIO.h>
#include <EEPROMex.h>
#include <EEPROMVar.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>
#include "constants.h"
#include "orders.h"
#include "coinAcceptor.h"
#include "scissor.h"
#include "shutter.h"
#include "paper.h"
#include "rtc.h"
#include "tests.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>

// Work variables
byte stepTakeShot = 0;
int freeSlot = 7;
storage parametres;
RF24 radio(RADIO_CE, RADIO_CSN); // CE, CSN
volatile char order = NO_ORDER;
float bathTemp = 0;
float lux = 0;
int tankTime = 18000;
bool ceilingOn = false; // Ceiling ligth switched on or not.
unsigned long lastCallSensor = 0;
char statusSpider = ' ';
byte exposureMode = 0; // 3 modes (0 = normal, 1 = double exposure, 2 = light painting).
bool bFirstShot = true; // for double exposure.

Output<ORDER_INTERRUPT_PIN> orderPause;
Adafruit_TSL2591 luxMeter = Adafruit_TSL2591(2591);

void setup() {

  #ifdef DEBUG_MODE
    Serial.begin(9600);
  #endif
  Serial2.begin(9600);

  // Pin to create pause on paper process
  orderPause.write(HIGH);

  /* Lux sensor */
  if(!luxMeter.begin()){
    debug("setup", "No TSL2591!");
  }

  //Gain config
  luxMeter.setGain(TSL2591_GAIN_MED);
  //Timing config
  luxMeter.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  
  //tests
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Mechanical counter
  pinMode(COUNT_PIN, OUTPUT);
  digitalWrite(COUNT_PIN, LOW);
  

  // Activate radio.
  radio.begin();
  radio.openWritingPipe(RADIO_ADRESS_EMITTER); // 00002
  radio.openReadingPipe(1, RADIO_ADRESS_RECEIVER); // 00001
  radio.setAutoAck(1);                    // Ensure autoACK is enabled so rec sends ack packet to let you know it got the transmit packet payload
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(250, 2);
  radio.maskIRQ(1,1,0);
  radio.startListening();

  // Seturp rtc in charge off litghting ceiling and marquee.
  setupRTC();

  EEPROM.readBlock(EEPROM_ADRESS, parametres);
  
  // Check verif code, if not correct init eeprom.
  if(parametres.checkCode != 123){
    parametres.checkCode = 123;
    parametres.totMoney = 0;
    parametres.totStrip = 0;
    parametres.mode = MODE_PAYING;
    parametres.price_cts = 200;
    parametres.free_price_cts = 100;
    EEPROM.writeBlock(EEPROM_ADRESS, parametres);
  }
  debug("parametres.mode", parametres.mode);
  parametres.mode = MODE_FREE; // TODO: remove after tests
  
  attachInterrupt(digitalPinToInterrupt(RADIO_INTERRUPT), radioInterrupt, LOW);
  showInitProgress(1);
  initPhotomaton();

  //TESTS: TODO: DELETE
}

void loop() {
  unsigned long currentMillis = millis();
showInitProgress(11);
  // Check every 10sec
  if(currentMillis - lastCallSensor > 10000){
    lastCallSensor = currentMillis;
    checkLuminosity();
    checkRTC();
    if(stepTakeShot == 0){
      sendOrderAndWait(ORDER_TEMP);
      sendOrderAndWait(ORDER_GET_TANK_TIME);
    }
  }
  showInitProgress(12);
  if(order == ENTER_TEST){
    detachInterrupt(digitalPinToInterrupt(RADIO_INTERRUPT));
    testMode(radio);
    statusSpider = RESPONSE_STATUS_RUNNING;
    order = NO_ORDER;
    attachInterrupt(digitalPinToInterrupt(RADIO_INTERRUPT), radioInterrupt, LOW);
  } 
showInitProgress(13);
  // If coin acceptor OK and clic start button.
  if(stepTakeShot == 0 && manageCoinsAndStart(parametres.mode)) {
    parametres.totStrip += 1;
    EEPROM.updateBlock(EEPROM_ADRESS, parametres);
    stepTakeShot = 1;
  }

  if(stepTakeShot > 0) {
    manageStepsTakeShot(); 
  } 
  showInitProgress(14);
}

void manageStepsTakeShot(){
  debug("manageStepsTakeShot-begin:", stepTakeShot);
  switch (stepTakeShot) {
    case 1: // First countdown
      startLedOff();
      showCountdown();
      while(getCountDown() > 0){
        refreshCountdown();
      }
      break;
      
    case 2: // Take photo.
    case 4:
    case 6:
    case 8:
      takeShot(exposureMode);
      break;
      
    case 3: // move paper for next shot + countdown
      movePaperNextShot(2);
      break;
    case 5:
      movePaperNextShot(3);
      break;
    case 7:
      movePaperNextShot(4);
      break;
      
    case 9:// last paper move, need to wait for spider to be at the correct position and scissor opened.  
      auxOff();
      checkRTC();// disable or not ceiling.
      ceilingRed();
      sendOrderAndWait(ORDER_NEW_SLOT);
      openScissor();
      movePaperOut();
      break;
      
    case 10: // cut paper and zou!
      closeScissor();
      openScissor();
      sendOrderAndWait(ORDER_PAPER_READY);
      break;
      
    case 12: // move paper back for first shot.
      closeScissor();
      movePaperFirstShot();
      break;
    case 13: // check for free slot.
      exposureMode = 0; // reinit mode to normal.
      sendOrderAndWait(ORDER_NB_FREE_SLOT);
      if(freeSlot == 0){
        coinSegmentFull();
        while(freeSlot == 0){ // wait indefinitely for a free slot.
          sendOrderAndWait(ORDER_NB_FREE_SLOT);
        }
      }
      enableCoinAcceptor(parametres.mode);
      break;
  }

  bool bNextStep = true;
  if(exposureMode == 1){ // double exposure
    if((stepTakeShot == 2 || stepTakeShot == 4 || stepTakeShot == 6 || stepTakeShot == 8)){
      // wait for clic on start button to take next shot.
      if(stepTakeShot != 8 || bFirstShot == true) { // no need to push button after all shot taken.
        waitForStart();
      }
      if(bFirstShot){
        bFirstShot = false;
        bNextStep = false;
        showCountdown();
        while(getCountDown() > 0){
          refreshCountdown();
        }
      } else {
        bFirstShot = true;
      }
    }
  }

  if(bNextStep){
    stepTakeShot = stepTakeShot >= 13 ? 0 : stepTakeShot + 1;
  }
}

boolean sendOrderAndWait(char sendOrder){
  debug("sendOrderAndWait-begin:", sendOrder);
  boolean bOK = false;
  
  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;

  while(!bOK){
    currentMillis = millis();
    // Send order to dev part and if no response for 10 second we send it again.
    if(currentMillis - lastMillis > 10000){
      debug("re order:", sendOrder);
      Serial2.print(sendOrder);
      Serial2.flush();
      lastMillis = currentMillis;
    }
    
    if (Serial2.available() > 0) {// if new response coming.
      
      switch(sendOrder){
        case ORDER_NEW_SLOT:{
          // Ready to send paper.
          char response = Serial2.read();
          bOK = response == ORDER_NEW_SLOT_READY;
          debug("new_slot:", bOK);
          break;
        }
        case ORDER_NB_FREE_SLOT:{ // Get number of free slot.
          freeSlot = Serial2.parseInt();
          debug("freeSlot:", freeSlot);
          bOK = true;
          break;
        }
        case ORDER_SPIDER_READY:{ // Spider is ready ?
          char response = Serial2.read();
          bOK = response == RESPONSE_OK || response == RESPONSE_STATUS_TEST;
          // In case spider is in test mode camera will follow into test
          order = response == RESPONSE_STATUS_TEST ? ENTER_TEST : NO_ORDER;
          debug("spider_ready", response);
          break;
        }
        case ORDER_TEMP:{ // Bath temp?
          bathTemp = Serial2.parseFloat();
          debug("temp:", bathTemp);
          bOK = true;
          break;
        }
        case ORDER_PAPER_READY:{ // Do the paper process got the message that paper as been delivered?
          char response = Serial2.read();
          bOK = response == RESPONSE_OK;
          debug("paper_ready:", bOK);
          break;
        }
        case ORDER_GET_STATUS:{ // get status from paper process.
          statusSpider = Serial2.read();
          bOK = true;
          debug("status:", statusSpider);
          break;
        }
        case ORDER_GET_TANK_TIME:{ // get tank time from paper process.
          tankTime = Serial2.parseInt();
          bOK = true;
          debug("tankTime:", tankTime);
          break;
        }
      }
    }
  }
  return bOK;
}

/***************************
 *  LUMINOSITY & LIGHTS
 **************************/
/*
 * Retrieve luminosity from the lux sensor 2591.
 * On low ligth, light up ceiling and billboard.
 * Ligth up flash or not during shot.
 * TODO: switch to a RTC.
 */
void checkLuminosity(){
  lux = luxMeter.getLuminosity(TSL2591_VISIBLE);;
  debug("lux:", lux);
}

/***************************
 *  INIT & STARTUP
 **************************/
void initPhotomaton(){
  debug("initPhotomaton-", String("begin"));
    
  // Coin acceptor off 
  disableCoinAcceptor();
  showInitProgress(2);
  // flash off
  flashOff();
  showInitProgress(3);
  // Start button off
  startLedOff();
  showInitProgress(4);
  // 4 * 7 segment display for coin
  initCoinSegment();
  showInitProgress(5);
  // Init steppers position.
  initScissor();
  showInitProgress(6);
  // Shutter
  initShutter();
  showInitProgress(7);
  // Paper
  initPaper();
   showInitProgress(8); 
  //Wait for paper process.
  sendOrderAndWait(ORDER_SPIDER_READY);
  showInitProgress(9);  
  enableCoinAcceptor(parametres.mode);
  showInitProgress(10);
  debug("initPhotomaton-", String("end"));
}

/*
 * Interrupt on incoming message from remote.
 */
void radioInterrupt(){
  if (radio.available()) {
      char tmp = NO_ORDER;
      radio.read(&tmp, sizeof(tmp));
      order = tmp;
      debug("radio order:", order);

      // Respond to quick orders.
      switch(order){
        case ORDER_PAUSE:
          digitalWrite(LED_BUILTIN, HIGH);
          orderPause.write(LOW);
          // Emergency stop, endless loop. Inside interrupt, fuck yeah!
          while(order == ORDER_PAUSE){
            if (radio.available()) {
              char tmp = NO_ORDER;
              radio.read(&tmp, sizeof(tmp));
              order = tmp;
              if(order == ORDER_GET_STATUS){
                byte state = RESPONSE_STATUS_PAUSE;
                radio.stopListening();
                radio.write(&state, sizeof(state));
                radio.flush_rx();
                radio.startListening();
                order = ORDER_PAUSE;
                
              }else if(order != ORDER_RESUME){
                order = ORDER_PAUSE;
              }
              Serial.println("resume");
            }
          }
          digitalWrite(LED_BUILTIN, LOW);
          orderPause.write(HIGH);
          order = NO_ORDER;
          break;
          
        case ORDER_TOTCENT:
          radio.stopListening();
          radio.write(&parametres.totMoney, sizeof(parametres.totMoney));
          radio.flush_rx();
          radio.startListening();
          order = NO_ORDER;
          break;
          
        case ORDER_NBSTRIP:
          radio.stopListening();
          radio.write(&parametres.totStrip, sizeof(parametres.totStrip));
          radio.flush_rx();
          radio.startListening();
          order = NO_ORDER;
          break;

        case ORDER_TEMP:
          radio.stopListening();
          radio.write(&bathTemp, sizeof(bathTemp));
          radio.flush_rx();
          radio.startListening();
          debug("bathTemp", bathTemp);
          order = NO_ORDER;
          break;

        case ORDER_MODE:
          radio.stopListening();
          radio.write(&parametres.mode, sizeof(parametres.mode));
          radio.flush_rx();
          radio.startListening();
          order = NO_ORDER;
          break;

        case ORDER_GET_STATUS:{
          byte state = statusSpider == RESPONSE_STATUS_TEST ? RESPONSE_STATUS_TEST : RESPONSE_STATUS_RUNNING;
          radio.stopListening();
          radio.write(&state, sizeof(state));
          radio.flush_rx();
          radio.startListening();
          order = state == RESPONSE_STATUS_TEST ? ENTER_TEST : NO_ORDER;
          break;
        }
        case ORDER_SET_MODE:
          delay(250);
          if (radio.available()) {
            byte newMode = MODE_PAYING;
            radio.read(&newMode, sizeof(newMode));
            parametres.mode = newMode;
            EEPROM.updateBlock(EEPROM_ADRESS, parametres);
          }
          order = NO_ORDER;
          break;
        case ORDER_GET_PRICE:
          radio.stopListening();
          radio.write(&parametres.price_cts, sizeof(parametres.price_cts));
          radio.flush_rx();
          radio.startListening();
          order = NO_ORDER;
          break;
        case ORDER_SET_PRICE:
          delay(250);
          if (radio.available()) {
            int price = 200;
            radio.read(&price, sizeof(price));
            parametres.price_cts = price;
            EEPROM.updateBlock(EEPROM_ADRESS, parametres);
          }
          order = NO_ORDER;
          break;
        case ORDER_GET_FREE_PRICE:
          radio.stopListening();
          radio.write(&parametres.free_price_cts, sizeof(parametres.free_price_cts));
          radio.flush_rx();
          radio.startListening();
          order = NO_ORDER;
          break;
        case ORDER_SET_FREE_PRICE:
          delay(250);
          if (radio.available()) {
            int freePrice = 100;
            radio.read(&freePrice, sizeof(freePrice));
            parametres.free_price_cts = freePrice;
            EEPROM.updateBlock(EEPROM_ADRESS, parametres);
          }
          order = NO_ORDER;
          break;
        case ORDER_GET_TANK_TIME:
          radio.stopListening();
          radio.write(&parametres.tank_time, sizeof(parametres.tank_time));
          radio.flush_rx();
          radio.startListening();
          order = NO_ORDER;
          break;
        case ORDER_SET_TANK_TIME:
          delay(250);
          if (radio.available()) {
            int tankTime = 18000;
            radio.read(&tankTime, sizeof(tankTime));
            parametres.tank_time = tankTime;
            EEPROM.updateBlock(EEPROM_ADRESS, parametres);
          }
          break;
        case ORDER_EXPOSURE_MODE:
          delay(250);
          if (radio.available()) {
            byte newExpoMode = 0;
            radio.read(&newExpoMode, sizeof(newExpoMode));
            exposureMode = newExpoMode;
          }
          // if the camera is not running start process.
          if(stepTakeShot == 0){
            parametres.totStrip += 1;
            EEPROM.updateBlock(EEPROM_ADRESS, parametres);
            stepTakeShot = 1;
          }
          order = NO_ORDER;
          break;
      }
    }
    debug("order", order);
}
