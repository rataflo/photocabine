#include "tests.h"

void testMode(){
  
  byte order = NO_ORDER;
  //Serial.println(order);
  while (order != EXIT_TEST){
    if(Serial.available() > 0){
      order = Serial.read();
    }
    switch(order){
      case EXIT_TEST:
        Serial.print(RESPONSE_OK);
        Serial.flush();
        break;
      case ORDER_GET_STATUS: // get status.
        Serial.print(RESPONSE_STATUS_TEST);
        Serial.flush();
        break;
      case ORDER_SWUP: // test upper switch.
        Serial.print(digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP));
        Serial.flush();
        break; 
      case ORDER_SWDOWN: // Test switch bottom
        Serial.print(digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM));
        Serial.flush();
        break;
      case ORDER_SWROTPAIR: // Test switch 1 rotation
        Serial.print(digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN));
        Serial.flush();
        break;
      case ORDER_SWROTIMPAIR: // Test switch 2 rotation
        Serial.print(digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN));
        Serial.flush();
        break;
      case ORDER_UPSPIDER: // Go spider to the top
        initSpiderUp();
        break;
      case ORDER_DOWNSPIDER: // Go spider to the top
        initSpiderBottom();
        break;
      case ORDER_ROTATESPIDER: // Rotate spider.
        byte fakeSlot[14];
        rotateSpider(fakeSlot);
        break;
      case ORDER_OPENARM: // Open arm
        openArm();
        break;
      case ORDER_CLOSEARM: // Close arm
        closeArm();
        break;
      case ORDER_IDLEARM: // Put the arm on idle pos.
        setServoArmWaitPos();
        break;
      case ORDER_DELIVERYRUN: // Run the delivery indefinitely.
        if(!isDeliveryRunning()){
          initDelivery();
          runDelivery();
        }
        else {
          stopDelivery();
        }
        break;
      case ORDER_LEDSTRIP: // flash led strip.
        lightFullStrip();
        break;
    }
    order = NO_ORDER;
  }
}

