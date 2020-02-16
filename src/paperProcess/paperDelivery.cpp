#include "paperDelivery.h"

AccelStepper stepperExit(1, SPIDER_EXIT_PIN_STP, SPIDER_EXIT_PIN_DIR);
bool deliveryRunning = false;

void setupDelivery(){
  pinMode(SPIDER_EXIT_PIN_ENABLE, OUTPUT);
}

void initDelivery(){
  
  digitalWrite(SPIDER_EXIT_PIN_ENABLE, HIGH);
  stepperExit.setMaxSpeed(SPIDER_ROTATE_SPEED);
  stepperExit.setAcceleration(SPIDER_ROTATE_ACCEL);
}

void runDelivery(byte *slots){
  downToMiddleSpider();
  blindRotate(slots);
  unsigned long startMillis = millis();
  unsigned long currentMillis = startMillis;
  
  digitalWrite(SPIDER_EXIT_PIN_ENABLE, LOW);
  deliveryRunning = true;
  int runPos = 0;
  while(currentMillis - startMillis < DELIVERY_TIME){
    stepperExit.moveTo(runPos); 
    stepperExit.run();
    asyncSpiderUp();
    currentMillis = millis();
    runPos++;
  }
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  stepperExit.stop();
  stepperExit.setCurrentPosition(0);
  stepperExit.run();
  digitalWrite(SPIDER_EXIT_PIN_ENABLE, HIGH);
  deliveryRunning = false;
}

bool isDeliveryRunning(){
  return deliveryRunning;
}
