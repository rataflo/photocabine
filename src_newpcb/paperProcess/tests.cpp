#include "tests.h"

void testMode(struct storage *params){
  Serial.println("testMode - begin");
  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;
  char testOrder = NO_ORDER;
  //Serial2.println(order);
  while (testOrder != EXIT_TEST){
    currentMillis = millis();
    
    if(Serial2.available() > 0){
      testOrder = Serial2.read();
      Serial.println(testOrder);
      serial2Clear();
    }
    
    switch(testOrder){
      case EXIT_TEST:
        Serial2.print(RESPONSE_OK);
        Serial2.flush();
        testOrder = EXIT_TEST;
        break;
      case ORDER_GET_STATUS: // get status.
        Serial2.print(RESPONSE_STATUS_TEST);
        Serial2.flush();
        testOrder = NO_ORDER;
        break;
      case ORDER_SPIDER_READY: // get status.
        Serial2.print(RESPONSE_STATUS_TEST);
        Serial2.flush();
        testOrder = NO_ORDER;
        break;
      case ORDER_UPSPIDER: // Go spider to the top
        initSpiderUp();
        testOrder = NO_ORDER;
        break;
      case ORDER_DOWNSPIDER: // Go spider to the top
        initSpiderBottom();
        testOrder = NO_ORDER;
        break;
      case ORDER_ROTATESPIDER: // Rotate spider.
        rotateSpider(params);
        testOrder = NO_ORDER;
        break;
      case ORDER_OPENARM: // Open arm
        openArm(params);
        testOrder = NO_ORDER;
        break;
      case ORDER_CLOSEARM: // Close arm
        closeArm(params);
        testOrder = NO_ORDER;
        break;
      case ORDER_IDLEARM: // Put the arm on idle pos.
        setServoArmWaitPos();
        testOrder = NO_ORDER;
        break;
      case ORDER_DELIVERYRUN: // Run the delivery indefinitely.
        runDelivery(params);
        testOrder = NO_ORDER;
        break;
      case ORDER_LEDSTRIP: // flash led strip.
        lightFullStrip();
        testOrder = NO_ORDER;
        break;
    }
  }
  Serial.println("testMode - end");
}

void sendAnswer(boolean answer){
  char transco = answer ? ORDER_TRUE : ORDER_FALSE;
  Serial.println(answer);
  Serial2.print(transco);
  Serial2.flush();
}

void debug(String functionName, String varValue){
  #ifdef DEBUG_MODE
    Serial.println(functionName + ":" + varValue);
  #endif
}

void debug(String functionName, boolean varValue){
  #ifdef DEBUG_MODE
    Serial.println(functionName + ":" + (varValue ? "true": "false"));
  #endif
}

void debug(String functionName, byte varValue){
  #ifdef DEBUG_MODE
    Serial.println(functionName + ":" + varValue);
  #endif
}

void serial2Clear(){
  while(Serial2.available() > 0) { 
    char t = Serial2.read(); 
  }
} 
