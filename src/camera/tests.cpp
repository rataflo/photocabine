#include "tests.h"


void testMode(RF24 radio){
  Serial.println("testMode - begin");
  // tell paper process to enter test mode.
  Serial2.print(ENTER_TEST);
  Serial2.flush();

  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;
  char testOrder = NO_ORDER;
  byte answer;
  
  radio.startListening();
  while (testOrder != EXIT_TEST){
    currentMillis = millis();
    
    // check for an incoming order.
    if (radio.available()) {
      radio.read(&testOrder, sizeof(testOrder));
      Serial.println(testOrder);
    }
    
    switch(testOrder){
      case EXIT_TEST:
        Serial2.print(EXIT_TEST);
        Serial2.flush();
        sendAnswer(radio, RESPONSE_OK);
        testOrder = EXIT_TEST;
        break;
      case ORDER_GET_STATUS:
      Serial.println("getStatus");
        sendAnswer(radio, RESPONSE_STATUS_TEST);
        testOrder = NO_ORDER;
        break;
      case ORDER_SWSHUTTER:{
        if(currentMillis - lastMillis > 1000){
          Serial.println("shutter");
          sendAnswer(radio, readSWShutter());
          lastMillis = currentMillis;
        }
      }
      break;
      case ORDER_SWSCISSOR:{
        if(currentMillis - lastMillis > 1000){
          Serial.println("shutter");
          sendAnswer(radio, readSWScissor());
          lastMillis = currentMillis;
        }
      }
      break;
      case ORDER_SWPAPER1:{
        if(currentMillis - lastMillis > 1000){
          Serial.println("shutter");
          sendAnswer(radio, readSWPaper1());
          lastMillis = currentMillis;
        }
      }
      break;
      case ORDER_SWPAPER2:{
        if(currentMillis - lastMillis > 1000){
          Serial.println("shutter");
          sendAnswer(radio, readSWPaper2());
          lastMillis = currentMillis;
        }
      }
      break;
      case ORDER_SWPAPER3:{
        if(currentMillis - lastMillis > 1000){
          Serial.println("shutter");
          sendAnswer(radio, readSWPaper3());
          lastMillis = currentMillis;
        }
      }
      break;
      case ORDER_SWPAPER4:{
        if(currentMillis - lastMillis > 1000){
          Serial.println("shutter");
          sendAnswer(radio, readSWPaper4());
          lastMillis = currentMillis;
        }
      }
      break;
      case ORDER_SWSTART:{
        if(currentMillis - lastMillis > 1000){
          Serial.println("start");
          sendAnswer(radio, readSWStart());
          lastMillis = currentMillis;
        }
      }
      break;
      case ORDER_SHUTTER:
        takeShot();
        testOrder = NO_ORDER;
        break;
      case ORDER_SCISSOR:{
        if(!readSWScissor()){
          closeScissor();
        } else {
          openScissor();
        }
        testOrder = NO_ORDER;
      }
      case ORDER_PAPER_FEEDER:
        initPaper();
        testOrder = NO_ORDER;
        break;
      case ORDER_FLASH:
        if(!isFlashOn()){
          flashOn();
        } else {
          flashOff();
        }
        testOrder = NO_ORDER;
        break;
      case ORDER_START_LIGHT:
        if(!isStartLedOn()){
          startLedOn();
        } else {
          startLedOff();
        }
        testOrder = NO_ORDER;
        break;
      case ORDER_SWUP :
      case ORDER_SWDOWN :
      case ORDER_SWROTPAIR :
      case ORDER_SWROTIMPAIR : // Switch orders for paper process
        if(currentMillis - lastMillis > 1000){
          Serial2.print(testOrder);
          Serial2.flush();
          // check answer from paper process.
          delay(250); // leave time to paper process to respond.
          if(Serial2.available() > 0){
            char transco = Serial2.read();
            Serial.println(transco);
            sendAnswer(radio, transco);
          }
          lastMillis = currentMillis;
        }
        break;
        
    }
    
    
  }
  Serial.println("testMode - end");
}

void sendAnswer(RF24 radio, boolean answer){
  Serial.println(answer);
  radio.stopListening();
  char transco = answer ? ORDER_TRUE : ORDER_FALSE;
  radio.write(&transco, sizeof(transco));
  radio.flush_rx();
  radio.startListening();
}

void sendAnswer(RF24 radio, char answer){
  Serial.println(answer);
  radio.stopListening();
  radio.write(&answer, sizeof(answer));
  radio.flush_rx();
  radio.startListening();
}
