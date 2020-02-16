/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 * 4* 7 segment, TM1637 : https://github.com/avishorp/TM1637
 * LCD Matrix 8*8 : http://wayoda.github.io/LedControl/
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DirectIO.h>
#include <EEPROMex.h>
#include <EEPROMVar.h>
#include "constants.h"
#include "orders.h"
#include "coinAcceptor.h"
#include "scissor.h"
#include "shutter.h"
#include "paper.h"
#include "tests.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2591.h>

// Work variables
byte stepTakeShot = 0;
int freeSlot = 7;
storage parametres;
RF24 radio(RADIO_CE, RADIO_CSN); // CE, CSN
volatile char order = NO_ORDER;
float bathTemp = 10;
float lux = 0;
int tankTime = 18000;
bool ceilingOn = false; // Ceiling ligth switched on or not.
unsigned long lastCallTemp = 0;
char statusSpider = ' ';

Output<ORDER_INTERRUPT_PIN> orderPause;
Adafruit_NeoPixel ceilingPixels = Adafruit_NeoPixel(CEILING_NBPIXEL, CEILING_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_TSL2591 luxMeter = Adafruit_TSL2591(2591);

void setup() {

  #ifdef DEBUG_MODE
    Serial.begin(9600);
  #endif
  Serial2.begin(9600);

  // Pin to create pause on paper process
  orderPause.write(HIGH);

  ceilingPixels.begin(); 

  /* Lux sensor */
  if(!luxMeter.begin()){
    debug("setup", "No TSL2591!");
  }

  //Gain config
  luxMeter.setGain(TSL2591_GAIN_MED);
  //Timing config
  luxMeter.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  checkLuminosity();
  
  //tests
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

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
  
  EEPROM.readBlock(EEPROM_ADRESS, parametres);
  
  // Check verif code, if not correct init eeprom.
  if(parametres.checkCode != 123){
    parametres.checkCode = 123;
    parametres.totMoney = 0;
    parametres.totStrip = 0;
    parametres.isRunning = false;
    parametres.mode = MODE_PAYING;
    parametres.price_cts = 200;
    parametres.free_price_cts = 100;
    EEPROM.writeBlock(EEPROM_ADRESS, parametres);
  }
  parametres.mode = MODE_FREE; // TODO: remove after tests

  // If dev part was previously running we start in test mode to force pause and avoid any problem.
  sendOrderAndWait(ORDER_GET_STATUS);
  bool bTest = (parametres.isRunning || statusSpider == RESPONSE_STATUS_TEST) ? true : false;
  if(bTest){
    testMode(radio);
    parametres.isRunning = false;
    EEPROM.updateBlock(EEPROM_ADRESS, parametres);
  }
  
  attachInterrupt(digitalPinToInterrupt(RADIO_INTERRUPT), radioInterrupt, LOW);
  
  initPhotomaton();
}

void loop() {
  checkLuminosity();
  debug("loop", order);
  if(order == ENTER_TEST){
    detachInterrupt(digitalPinToInterrupt(RADIO_INTERRUPT));
    testMode(radio);
    order = NO_ORDER;
    attachInterrupt(digitalPinToInterrupt(RADIO_INTERRUPT), radioInterrupt, LOW);
  } 

  // If coin acceptor OK and clic start button.
  if(stepTakeShot == 0 && manageCoinsAndStart(parametres.mode)) {
    parametres.totStrip += 1;
    parametres.isRunning = true;
    EEPROM.updateBlock(EEPROM_ADRESS, parametres);
    stepTakeShot = 1;
  }

  if(stepTakeShot > 0) {
    manageStepsTakeShot(); 
  } else{
    // When nothing to do ask for dev parameters every 10 sec.
    unsigned long currentMillis = millis();
    if(currentMillis - lastCallTemp > 10000){
      lastCallTemp = currentMillis;
      sendOrderAndWait(ORDER_TEMP);
      sendOrderAndWait(ORDER_GET_TANK_TIME);  
    }
  }

}

void manageStepsTakeShot(){
  debug("manageStepsTakeShot-begin:", stepTakeShot);
  switch (stepTakeShot) {
    case 1: // First countdown
      showCountdown();
      while(getCountDown() > 0){
        refreshCountdown();
      }
      break;
      
    case 2: // Take photo.
    case 4:
    case 6:
      takeShot();
      break;
    case 8:
      takeShot();
      startLedOff();
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
      sendOrderAndWait(ORDER_NEW_SLOT);
      openScissor();
      movePaperOut();
      break;
      
    case 10: // cut paper and zou!
      //closeScissor(); TODO: remove me!
      sendOrderAndWait(ORDER_PAPER_READY);
      break;
      
    case 12: // move paper back for first shot.
      movePaperFirstShot();
      break;
    case 13: // check for free slot.
      parametres.isRunning = false;
      EEPROM.updateBlock(EEPROM_ADRESS, parametres);
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
  stepTakeShot = stepTakeShot >= 13 ? 0 : stepTakeShot + 1;
}

boolean sendOrderAndWait(char sendOrder){
  debug("sendOrderAndWait-begin:", sendOrder);
  boolean bOK = false;
  
  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;

  while(!bOK){
    currentMillis = millis();
    // Send order to dev part and if no response for 10 second we send it again.
    if(lastMillis == 0 || currentMillis - lastMillis > 10000){
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
          bOK = response == RESPONSE_OK;
          // In case spider is in test mode camera will follow into test
          if(response == RESPONSE_STATUS_TEST){
            order = ENTER_TEST;
          }
          debug("spider_ready:", response);
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
  // The sun is down
  if(!ceilingOn && lux < 4){
    ceilingOn = true;
    // Ligth the ceiling
    for(int i=0;i<CEILING_NBPIXEL;i++){
      ceilingPixels.setPixelColor(i, 255,255,255); // white as hell
    }
    ceilingPixels.show();
  } else if(ceilingOn && lux > 30){
    ceilingOn = false;
    // switch off the ceiling
    for(int i=0;i<CEILING_NBPIXEL;i++){
      ceilingPixels.setPixelColor(i, 0, 0, 0); // Dark
    }
    ceilingPixels.show();
  }
}

/***************************
 *  INIT & STARTUP
 **************************/
void initPhotomaton(){
  debug("initPhotomaton-", String("begin"));
    
  // Coin acceptor off 
  disableCoinAcceptor();
  
  // flash off
  flashOff();
  
  // Start button off
  startLedOff();
  
  // 4 * 7 segment display for coin
  initCoinSegment();
  
  // Init steppers position.
  initScissor();
  
  // Shutter
  initShutter();
  
  // Paper
  initPaper();
    
  //Wait for paper process.
  sendOrderAndWait(ORDER_SPIDER_READY);
    
  enableCoinAcceptor(parametres.mode);
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
          radio.startListening();
          order = NO_ORDER;
          break;
          
        case ORDER_NBSTRIP:
          radio.stopListening();
          radio.write(&parametres.totStrip, sizeof(parametres.totStrip));
          radio.startListening();
          order = NO_ORDER;
          break;

        case ORDER_TEMP:
          debug("bathTemp:", bathTemp);
          radio.stopListening();
          radio.write(&bathTemp, sizeof(bathTemp));
          radio.startListening();
          order = NO_ORDER;
          break;

        case ORDER_MODE:
          radio.stopListening();
          radio.write(&parametres.mode, sizeof(parametres.mode));
          radio.startListening();
          order = NO_ORDER;
          break;

        case ORDER_GET_STATUS:{
          byte state = statusSpider == RESPONSE_STATUS_TEST ? RESPONSE_STATUS_TEST : RESPONSE_STATUS_RUNNING;
          radio.stopListening();
          radio.write(&state, sizeof(state));
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
      }
    }
    debug("order", order);
}
