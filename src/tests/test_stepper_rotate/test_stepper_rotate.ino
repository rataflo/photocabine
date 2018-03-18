#include <AccelStepper.h>
#include <MultiStepper.h>


#define SPIDER_ROTATE_PIN_STP 32
#define SPIDER_ROTATE_PIN_DIR 33

AccelStepper rotate(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);

void setup() {
  // put your setup code here, to run once:
rotate.setMaxSpeed(1000);
  rotate.setAcceleration(500);
  rotate.moveTo(-5684);//406 step for 1 tank move
}

void loop() {
  // put your main code here, to run repeatedly:
rotate.run();

}
