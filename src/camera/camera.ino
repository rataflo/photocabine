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

// Work variables
byte stepTakeShot = 0;
int freeSlot = 7;
volatile storage parametres;
RF24 radio(RADIO_CE, RADIO_CSN); // CE, CSN
volatile char order = NO_ORDER;
volatile int bathTemp = 10;

Output<ORDER_INTERRUPT_PIN> orderPause;

void setup() {
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
    Serial.println("init eeprom");
    parametres.checkCode = 123;
    parametres.totMoney = 0;
    parametres.totStrip = 0;
    parametres.isRunning = false;
    parametres.mode = MODE_PAYING;
    EEPROM.writeBlock(EEPROM_ADRESS, parametres);
  }
  
  // If the photomaton was previously running we start in test mode to force pause and avoid any problem.
  //if(parametres.isRunning){
    detachInterrupt(digitalPinToInterrupt(2));
    testMode(radio);
    parametres.isRunning = false;
    EEPROM.updateBlock(EEPROM_ADRESS, parametres);
    attachInterrupt(digitalPinToInterrupt(2), check_radio, LOW);
  //}
  //initPhotomaton();
}

void loop() {
  if(order == ENTER_TEST){
    detachInterrupt(digitalPinToInterrupt(2));
    testMode(radio);
    order = NO_ORDER;
    attachInterrupt(digitalPinToInterrupt(2), check_radio, LOW);
  }
  
  // Every minute we ask for bath temp.
  sendOrderAndWait(ORDER_TEMP);
  
  // If coin acceptor OK and clic start button.
  if(stepTakeShot == 0 && manageCoinsAndStart(parametres)) {
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
      enableCoinAcceptor(parametres);
      break;
  }
  stepTakeShot = stepTakeShot >= 13 ? 0 : stepTakeShot + 1;
}

boolean sendOrderAndWait(char order){

  boolean bOK = false;
  
  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;
  
  while(!bOK){
    currentMillis = millis();
    // if no response for 1 second we send the order again.
    if(currentMillis - lastMillis > 1000){
      Serial2.print(order);
      Serial2.flush();
      lastMillis = currentMillis;
    }
    
    if (Serial2.available() > 0) {// if new response coming.
      switch(order){
        case ORDER_NEW_SLOT:{
          // Ready to send paper.
          char response = Serial2.read();
          bOK = response == RESPONSE_OK;
          break;
        }
        case ORDER_NB_FREE_SLOT:{ // Get number of free slot.
          freeSlot = Serial2.parseInt();
          Serial.println(freeSlot);
          //freeSlot = response - '0';
          bOK = true;
          break;
        }
        case ORDER_SPIDER_READY:{ // Spider is ready ?
          char response = Serial2.read();
          bOK = response == RESPONSE_OK;
          break;
        }
        case ORDER_TEMP:{ // Bath temp?
          bathTemp = Serial2.parseFloat();
          Serial.println(bathTemp);
          //bathTemp = response - '0';
          bOK = true;
          break;
        }
      }
    }
  }
  return bOK;
}

/***************************
 *  INIT
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
    initPaper();
    
    //Wait for paper process.
    sendOrderAndWait(ORDER_SPIDER_READY);
    
    enableCoinAcceptor(parametres);
}

/*
 * Interrupt on incoming message from remote.
 */
void check_radio(){
  Serial.println("check_radio");
  if (radio.available()) {
      radio.read(&order, sizeof(order));
      Serial.println(order);

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

       case ORDER_GET_STATUS:
          byte answer = RESPONSE_STATUS_RUNNING;
          radio.stopListening();
          radio.write(&answer, sizeof(answer));
          radio.startListening();
          order = NO_ORDER;
          break;

        case ORDER_SET_MODE:
          delay(250);
          if (radio.available()) {
            byte newMode = MODE_PAYING;
            radio.read(&newMode, sizeof(newMode));
            parametres.mode = newMode;
            EEPROM.updateBlock(EEPROM_ADRESS, parametres);
          }
      }
      
    }
}

