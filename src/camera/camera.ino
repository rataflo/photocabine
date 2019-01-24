/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 * 4* 7 segment, TM1637 : https://github.com/avishorp/TM1637
 * LCD Matrix 8*8 : http://wayoda.github.io/LedControl/
 */
#include <DirectIO.h>
#include <Adafruit_NeoPixel.h>
#include "constants.h"
#include "coinAcceptor.h"
#include "scissor.h"
#include "shutter.h"
#include "paper.h"
#include "remote.h"
#include "tests.h"

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(78, 8, NEO_GRB + NEO_KHZ800);

// Work variables
byte stepTakeShot = 0;
int freeSlot = 7;
bool bGoTakeShot = false; 

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  pixels.begin(); 
  for(int i=0;i<78;i++){
    pixels.setPixelColor(i, pixels.Color(100,0,0)); 
    pixels.show();
  }
  
  initPhotomaton();
}

void loop() {

  // If coin acceptor OK and clic start button.
  if(manageCoinsAndStart()) {
    bGoTakeShot = true;
    stepTakeShot = 1;
  }

  if(bGoTakeShot) {
    manageStepsTakeShot(); 
  }
  
  // if shot finished and coin acceptor disabled.
  if(!bGoTakeShot && !isCoinEnabled()){
    stepTakeShot = 0;
    enableCoinAcceptor();
  }
  
  checkMenu();
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
      bGoTakeShot = false; // Finish
      break;
  }
  stepTakeShot++;
}

/***************************
 *    INITS
 **************************/
void initPhotomaton(){
  
  // Coin acceptor off 
  disableCoinAcceptor();

  initRemote();

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
  
  //Cait for spider.
  sendOrderAndWait(ORDER_SPIDER_READY);

  enableCoinAcceptor();
  showMainScreen();
}

boolean sendOrderAndWait(char order){

  char response;
  boolean bOK = false;
  
  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;
  
  while(!bOK){
    currentMillis = millis();
    // if no response for 1 second we send the order again.
    if(currentMillis - lastMillis > 1000){
      Serial1.print(order);
      Serial1.flush();
      lastMillis = currentMillis;
    }
  
    if (Serial1.available() > 0) {// if new response coming.
      response = Serial1.read();
    
      switch(order){
        case ORDER_NEW_SLOT:
          // Ready to send paper.
          bOK = response == RESPONSE_OK;
          break;
    
        case ORDER_NB_FREE_SLOT: // Get number of free slot.
          freeSlot = response - '0';
          bOK = true;
          break;

        case ORDER_SPIDER_READY: // Spider is ready.
          bOK = response == RESPONSE_OK;
          break;
      }
    }
  }
  return bOK;
}

