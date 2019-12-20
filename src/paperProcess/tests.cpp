#include "tests.h"

void testMode(){
  Serial.println("testMode - begin");
  unsigned long lastMillis = 0;
  unsigned long currentMillis = 0;
  byte testOrder = NO_ORDER;
  //Serial2.println(order);
  while (testOrder != EXIT_TEST){
    currentMillis = millis();
    
    if(Serial2.available() > 0){
      testOrder = Serial2.read();
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
      case ORDER_SWUP: // test upper switch.
        if(currentMillis - lastMillis > 1000){
          sendAnswer(digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP));
          lastMillis = currentMillis;
        }
        break; 
      case ORDER_SWDOWN: // Test switch bottom
        if(currentMillis - lastMillis > 1000){
          sendAnswer(digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM));
          lastMillis = currentMillis;
        }
        break;
      case ORDER_SWROTPAIR: // Test switch 1 rotation
        if(currentMillis - lastMillis > 1000){
          sendAnswer(digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN));
          lastMillis = currentMillis;
        }
        break;
      case ORDER_SWROTIMPAIR: // Test switch 2 rotation
        if(currentMillis - lastMillis > 1000){
          sendAnswer(digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN));
          lastMillis = currentMillis;
        }
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
        byte fakeSlot[14];
        rotateSpider(fakeSlot);
        testOrder = NO_ORDER;
        break;
      case ORDER_OPENARM: // Open arm
        openArm();
        testOrder = NO_ORDER;
        break;
      case ORDER_CLOSEARM: // Close arm
        closeArm();
        testOrder = NO_ORDER;
        break;
      case ORDER_IDLEARM: // Put the arm on idle pos.
        setServoArmWaitPos();
        testOrder = NO_ORDER;
        break;
      case ORDER_DELIVERYRUN: // Run the delivery indefinitely.
        if(!isDeliveryRunning()){
          initDelivery();
          runDelivery();
        }
        else {
          stopDelivery();
        }
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
