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

// Work variables
byte stepTakeShot = 0;
int freeSlot = 7;
volatile storage parametres;
RF24 radio(RADIO_CE, RADIO_CSN); // CE, CSN
volatile char order = NO_ORDER;
volatile float bathTemp = 10;

Output<ORDER_INTERRUPT_PIN> orderPause;
Adafruit_NeoPixel ceilingPixels = Adafruit_NeoPixel(CEILING_NBPIXEL, CEILING_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  // Ligth the ceiling
  ceilingPixels.begin(); 
  for(int i=0;i<CEILING_NBPIXEL;i++){
    ceilingPixels.setPixelColor(i, 255,255,255); // white as hell
    ceilingPixels.show();
  }
  
  Serial.begin(9600);
  Serial2.begin(9600);

  // Pin to create pause on paper process
  orderPause.write(HIGH);
  
  //tests
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  radio.begin();
  radio.openWritingPipe(RADIO_ADRESS_EMITTER); // 00002
  radio.openReadingPipe(1, RADIO_ADRESS_RECEIVER); // 00001
  radio.setAutoAck(1);                    // Ensure autoACK is enabled so rec sends ack packet to let you know it got the transmit packet payload
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(250, 2);
  radio.maskIRQ(1,1,0);
  radio.startListening();

  // Interrupt for radio message.
  attachInterrupt(digitalPinToInterrupt(2), check_radio, LOW);
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
    parametres.tank_time = 18000;
    EEPROM.writeBlock(EEPROM_ADRESS, parametres);
  }

  // If the photomaton was previously running we start in test mode to force pause and avoid any problem.
  bool bTest = (parametres.isRunning || sendOrderAndWait(ORDER_GET_STATUS) == RESPONSE_STATUS_TEST) ? true : false;
  if(bTest){
    detachInterrupt(digitalPinToInterrupt(2));
    testMode(radio);
    parametres.isRunning = false;
    EEPROM.updateBlock(EEPROM_ADRESS, parametres);
    attachInterrupt(digitalPinToInterrupt(2), check_radio, LOW);
  }
  initPhotomaton(); // TODO: remove comment when test are over.
}

void loop() {
  
  if(order == ENTER_TEST){
    detachInterrupt(digitalPinToInterrupt(2));
    testMode(radio);
    attachInterrupt(digitalPinToInterrupt(2), check_radio, LOW);
  
  } else if(order == ORDER_SET_TANK_TIME){
    Serial2.print(ORDER_SET_TANK_TIME);
    Serial2.flush();
    Serial2.print(parametres.tank_time);
    Serial2.flush();
    order = NO_ORDER;
  }

  // Every minute we ask for bath temp.
  sendOrderAndWait(ORDER_TEMP); 

  // If coin acceptor OK and clic start button.
  if(stepTakeShot == 0 && manageCoinsAndStart(parametres.mode)) {
    parametres.totStrip += 1;
    parametres.isRunning = true;
    EEPROM.updateBlock(EEPROM_ADRESS, parametres);
    stepTakeShot = 1;
  }

  if(stepTakeShot > 0) {
    manageStepsTakeShot(); 
  }

}

void manageStepsTakeShot(){
  Serial.println("manageStepsTakeShot");
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
      closeScissor();
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
  Serial.print("sendOrderAndWait - begin:");Serial.println(sendOrder);
  boolean bOK = false;
  
  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;
  
  while(!bOK){
    currentMillis = millis();
    // if no response for 1 second we send the order again.
    if(currentMillis - lastMillis > 1000){
      Serial2.print(sendOrder);
      Serial2.flush();
      lastMillis = currentMillis;
    }
    
    if (Serial2.available() > 0) {// if new response coming.
      Serial.print("response:");Serial.println(sendOrder);
      switch(sendOrder){
        case ORDER_NEW_SLOT:{
          // Ready to send paper.
          char response = Serial2.read();
          bOK = response == ORDER_NEW_SLOT_READY;
          break;
        }
        case ORDER_NB_FREE_SLOT:{ // Get number of free slot.
          freeSlot = Serial2.parseInt();
          Serial.println(freeSlot);
          bOK = true;
          break;
        }
        case ORDER_SPIDER_READY:{ // Spider is ready ?
          char response = Serial2.read();
          bOK = response == RESPONSE_OK;
          break;
        }
        case ORDER_TEMP:{ // Bath temp?
          Serial.println("temp=");
          bathTemp = Serial2.parseFloat();
          Serial.println(bathTemp);
          bOK = true;
          break;
        }
        case ORDER_PAPER_READY:{ // Do the paper process got the message that paper as been delivered?
          char response = Serial2.read();
          bOK = response == RESPONSE_OK;
          break;
        }
        case ORDER_GET_STATUS:{ // get status from paper process.
          char response = Serial2.read();
          bOK = true;
          break;
        }
      }
    }
  }
  return bOK;
}

/***************************
 *  INIT & STARTUP
 **************************/
void initPhotomaton(){
  
    // Coin acceptor off 
    disableCoinAcceptor();
  
    // flash off
    flashOff();
  
    // Start button off
    startLedOff();
  
    // 4 * 7 segment display for coin
    initCoinSegment();
  
    // Led Matrix
    initLedMatrix();
  
    // Init steppers position.
    initScissor();
  
    // Shutter
    initShutter();
  
    // Paper
    //initPaper();
    
    //Wait for paper process.
    /*sendOrderAndWait(ORDER_SPIDER_READY);*/
    
    enableCoinAcceptor(parametres.mode);
}

/*
 * Interrupt on incoming message from remote.
 */
void check_radio(){
  Serial.println("check_radio");
  if (radio.available()) {
      radio.read(&order, sizeof(order));
      Serial.println(order);

      // Respond to quick orders.
      switch(order){
        case ORDER_PAUSE:
          digitalWrite(LED_BUILTIN, HIGH);
          orderPause.write(LOW);
          // Emergency stop, endless loop. Inside interrupt, fuck yeah
          while(order == ORDER_PAUSE){
            if (radio.available()) {
              radio.read(&order, sizeof(order));
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
          Serial.println("temp");
          Serial.println(bathTemp);
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
          byte state = RESPONSE_STATUS_RUNNING;
          radio.stopListening();
          radio.write(&state, sizeof(state));
          radio.startListening();
          order = NO_ORDER;
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
            Serial.println(price);
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
}
