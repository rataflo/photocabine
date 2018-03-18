/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 */
#include <DirectIO.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include "constants.h"

/*
 * GLOBAL VARIABLES
 */
byte states[7] = {0, 0, 0, 0, 0, 0, 0}; // Step of differents photo.
byte arms[7] = {0, 0, 0, 0, 0, 0, 0}; // State of arms (0 = no photo, > 0 = number of the photo related to states[])

unsigned long currentMillis = 0;

// Work variables
bool bInitPhotomaton = false;
bool bReadySpider = false; // true when spider is ok to receive a strip of paper

#include "servoArm.h"
#include "spider.h"
#include "tests.h"

void setup() {
  Serial.begin(9600);
  
  // DC motor spider up/down
  spiderUpPin.write(HIGH); // HIGH is off...
  spiderDownPin.write(HIGH);
  
  // stepper spider rotation
  spiderRotate.setMaxSpeed(1000);
  spiderRotate.setAcceleration(500);

  // servo arm position
  servoArm.attach(SERVO_ARM);
  servoArm.write(SERVO_ARM_IDLE_POS);  
    
  initPhotomaton();
}

void loop() {
  currentMillis = millis();

  checkMenu();
 
  scissor.run();
  spiderRotate.run();
  
}


/***************************
 *    INIT
 **************************/
void initPhotomaton(){

  // Spider
  initSpiderBottom();
  initSpiderUp();
  initRotate();
  // Init Servo
  setServoArmWaitPos();
  
  bInitPhotomaton = true;
}

