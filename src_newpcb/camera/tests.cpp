#include "tests.h"


void testMode(RF24 radio){
  debug("testMode-begin", "");
  // tell paper process to enter test mode.
  Serial2.print(ENTER_TEST);
  Serial2.flush();

  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;
  char testOrder = NO_ORDER;
  
  radio.startListening();
  while (testOrder != EXIT_TEST){
    currentMillis = millis();
    
    // check for an incoming order.
    if (radio.available()) {
      radio.read(&testOrder, sizeof(testOrder));
      debug("testOrder", testOrder);
    }
    
    switch(testOrder){
      case EXIT_TEST:
        Serial2.print(EXIT_TEST);
        Serial2.flush();
        sendAnswer(radio, RESPONSE_OK);
        testOrder = EXIT_TEST;
        break;
      case ORDER_GET_STATUS:
        sendAnswer(radio, RESPONSE_STATUS_TEST);
        testOrder = NO_ORDER;
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
    }
  }
 debug("testMode", String("end"));
}

void sendAnswer(RF24 radio, boolean answer){
  debug("sendAnswer-boolean", "");
  radio.stopListening();
  char transco = answer ? ORDER_TRUE : ORDER_FALSE;
  radio.write(&transco, sizeof(transco));
  radio.flush_rx();
  radio.startListening();
}

void sendAnswer(RF24 radio, char answer){
  debug("sendAnswer-char", "");
  radio.stopListening();
  radio.write(&answer, sizeof(answer));
  radio.flush_rx();
  radio.startListening();
}

void debug(String functionName, String varValue){
  
#ifdef DEBUG_MODE
  Serial.println(functionName + ":" + varValue);
  #endif
}
void debug(String functionName, float varValue){
  
#ifdef DEBUG_MODE
  Serial.println(functionName + ":" + String(varValue, 1));
  #endif
}
void debug(String functionName, char varValue){
  
#ifdef DEBUG_MODE
  Serial.println(functionName + ":" + varValue);
  #endif
}
void debug(String functionName, int varValue){
  
#ifdef DEBUG_MODE
  Serial.println(functionName + ":" + varValue);
  #endif
}

void debug(String functionName, boolean varValue){
  #ifdef DEBUG_MODE
    Serial.println(functionName + ":" + (varValue? "true" : "false"));
  #endif
}
