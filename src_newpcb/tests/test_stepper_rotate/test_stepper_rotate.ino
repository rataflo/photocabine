#include <AccelStepper.h>
#include <MultiStepper.h>

/* Calculate number of step do go from one tank to another */
#define SPIDER_ROTATE_PIN_STP 9
#define SPIDER_ROTATE_PIN_DIR 8
#define SPIDER_ROTATE_NBSTEP 305 // Number of steps to move to one tank
#define SPIDER_ROTATE_SPEED 600
#define SPIDER_ROTATE_ACCEL 100
#define SPIDER_ROTATE_PIN_ENABLE 7
#define SPIDER_ROTATE_PIN_M0 6
#define SPIDER_ROTATE_ENDSTOP2_PIN 33

AccelStepper spiderRotate(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);

void setup() {
  Serial.begin(9600);
  // spider rotation
  pinMode(SPIDER_ROTATE_PIN_M0, OUTPUT);
  digitalWrite(SPIDER_ROTATE_PIN_M0, HIGH);// Half step
  pinMode(SPIDER_ROTATE_PIN_ENABLE, OUTPUT);
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);

  initRotate();

  // put your main code here, to run repeatedly:
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);
  int delta = SPIDER_ROTATE_NBSTEP;
  spiderRotate.moveTo(delta);

  while (spiderRotate.currentPosition() != delta){
    spiderRotate.run();
  }
  spiderRotate.stop();
  spiderRotate.setCurrentPosition(0);
  spiderRotate.run();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}

void loop() {
  Serial.println("loop");
}

void initRotate() {
  Serial.println("initRotate-begin"); 
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);

  boolean bEndStop = !digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN);
  int homing = 1;
  while (!bEndStop) { 
    spiderRotate.moveTo(homing); 
    spiderRotate.run();
    homing++;
    delay(5);
    bEndStop = !digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN);
  }
  spiderRotate.setCurrentPosition(0);
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}
  
