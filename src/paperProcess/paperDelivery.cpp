#include "paperDelivery.h"

AccelStepper stepperExit(1, SPIDER_EXIT_PIN_STP, SPIDER_EXIT_PIN_DIR);

void initDelivery(){
  pinMode(SPIDER_EXIT_PIN_ENABLE, OUTPUT);
  digitalWrite(SPIDER_EXIT_PIN_ENABLE, HIGH);
  stepperExit.setMaxSpeed(SPIDER_ROTATE_SPEED);
  stepperExit.setAcceleration(SPIDER_ROTATE_ACCEL);
}

void runDelivery(){
  //we only go backward for a period of time. The stepper is not mandatory and can be a dc motor. I got a spare stepper on hand...
  digitalWrite(SPIDER_EXIT_PIN_ENABLE, LOW);
  stepperExit.setCurrentPosition(0);
  stepperExit.moveTo(1);
  stepperExit.run();
}

void stopDelivery(){
  stepperExit.stop();
  stepperExit.setCurrentPosition(0);
  stepperExit.run();
  digitalWrite(SPIDER_EXIT_PIN_ENABLE, HIGH);
}

