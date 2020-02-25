#include <AccelStepper.h>
#include <MultiStepper.h>

#define SPIDER_ROTATE_PIN_DIR 13
#define SPIDER_ROTATE_PIN_STP 10


AccelStepper rotate(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);

void setup() {
  // put your setup code here, to run once:
  rotate.setMaxSpeed(100);
  rotate.setAcceleration(100);
  rotate.moveTo(425);//406 step for 1 tank move
}

void loop() {
  // put your main code here, to run repeatedly:
rotate.run();

}
