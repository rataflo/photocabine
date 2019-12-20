#include "tests.h"


void testMode(RF24 radio){
  Serial.println("testMode");
  char order = NO_ORDER;
  byte answer;
  while (order != EXIT_TEST){
    // check for an incoming order.
    radio.startListening();
    delay(10); // leave time to receive message.
    if (radio.available()) {
      radio.read(&order, sizeof(order));
      Serial.print("ORDER=");Serial.println(order);
    }
    
    switch(order){
      case ENTER_TEST:
        Serial2.print(ENTER_TEST);
        Serial2.flush();
        break;
      case EXIT_TEST:
        Serial2.print(EXIT_TEST);
        Serial2.flush();
        break;
      case ORDER_GET_STATUS:
        answer = RESPONSE_STATUS_TEST;
        radio.stopListening();
        radio.write(&answer, sizeof(answer));
        break;
      case ORDER_SWSHUTTER:{
        bool swState = readSWShutter();
        radio.stopListening();
        radio.write(&swState, sizeof(swState));
      }
      break;
      case ORDER_SWSCISSOR:{
        bool swState = readSWScissor();
        radio.stopListening();
        radio.write(&swState, sizeof(swState));
      }
      break;
      case ORDER_SWPAPER1:{
        bool swState = readSWPaper1();
        radio.stopListening();
        radio.write(&swState, sizeof(swState));
      }
      break;
      case ORDER_SWPAPER2:{
        bool swState = readSWPaper2();
        radio.stopListening();
        radio.write(&swState, sizeof(swState));
      }
      break;
      case ORDER_SWPAPER3:{
        bool swState = readSWPaper3();
        radio.stopListening();
        radio.write(&swState, sizeof(swState));
      }
      break;
      case ORDER_SWPAPER4:{
        bool swState = readSWPaper4();
        radio.stopListening();
        radio.write(&swState, sizeof(swState));
      }
      break;
      case ORDER_SWSTART:{
        bool swState = readSWStart();
        radio.stopListening();
        radio.write(&swState, sizeof(swState));
      }
      break;
      case ORDER_SHUTTER:
        takeShot();
        break;
      case ORDER_SCISSOR:{
        if(!readSWScissor()){
          closeScissor();
        } else {
          openScissor();
        }
      }
      case ORDER_PAPER_FEEDER:
        initPaper();
        break;
      case ORDER_FLASH:
        if(!isFlashOn()){
          flashOn();
        } else {
          flashOff();
        }
        break;
      case ORDER_START_LIGHT:
        if(!isStartLedOn()){
          startLedOn();
        } else {
          startLedOff();
        }
        break;
    }
    
    // check answer from paper process.
    delay(250); // leave time to paper process to respond.
    if(Serial2.available() > 0){
      answer = Serial2.read();
      radio.stopListening();
      radio.write(&answer, sizeof(answer));
    }
    Serial.println(order);
  }
  radio.startListening();
}
