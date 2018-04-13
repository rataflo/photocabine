#include <AccelStepper.h>
#include <MultiStepper.h>

#define SPIDER_ROTATE_PIN_DIR 24
#define SPIDER_ROTATE_PIN_STP 25


AccelStepper stepper(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);

void setup() {
  // spider rotation
  /*stepper.setMaxSpeed(1000);
  stepper.setAcceleration(400);
  stepper.moveTo(1000);//406 step for 1 tank move*/

  //shutter
  stepper.setMaxSpeed(400);
  stepper.setAcceleration(100);
  stepper.moveTo(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
stepper.run();
if(stepper.currentPosition() == 1000){
  stepper.setCurrentPosition(0);
  stepper.setMaxSpeed(400);
  stepper.setAcceleration(100);
  stepper.moveTo(1000);
}
}
