/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 */
#include <Wire.h>
#include <DirectIO.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include "constants.h"

/*
 * GLOBAL VARIABLES
 */
byte positions[14] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0}; // State of a position in reverse clock order (0 = nothing, 1 = arm whitout photo, 2 = arm with photo)
byte arm0Pos = 0; // Position of the first arm.
byte freeSlot = 7; // number of free arms.
boolean bCheckAvailability = false;
boolean bCanProcess = false;
unsigned long currentMillis = 0;

// Work variables
bool bInitPhotomaton = false;
bool bSpiderUp = true; // true when spider is at the top.
char lastOrder = '0';

// time to go from bottom to upper switch.
unsigned long timeToGoUp = 0;
// time to go from upper switch to bottom switch.
unsigned long timeToGoDown = 0;

#include "servoArm.h"
#include "spider.h"
#include "tests.h"

void setup() {
  Serial.begin(9600);
  Wire.begin(11);
  Wire.onReceive(receiveOrder); 
  Wire.onRequest(respondToOrder); 
  
  // servo arm position
  servoArm.attach(SERVO_ARM);
  servoArm.write(SERVO_ARM_IDLE_POS);  

  // pwm dc up and down (do not use DirectIO)
  pinMode(SPIDER_UPDOWN_PIN_PWM, OUTPUT);
  
  initSpider();
}

void loop() {
  currentMillis = millis();

  //checkOrder();

  // Master request to give a strip to process.
  if(bSpiderUp && bCheckAvailability){
    checkAvailability();
  }

  if(bCanProcess){
    process();
  }

  spiderRotate.run();
}

/*
 * Check if master arduino send an order. Mainly to process a strip.
 */
void receiveOrder(){
  if(Wire.available()) {
    lastOrder = Wire.read();
  }
}

void respondToOrder(){
  if(lastOrder != '0') {
    if(lastOrder == 'G') { // G stand for 'Give me a place for processing my strip'.
      bCheckAvailability = true;
      
    } else if(lastOrder == 'O'){ // O stand for 'Oooo yeah i finished to give you the strip, do your job!'
      bCanProcess = true; 
      freeSlot--;
      
    } else if(lastOrder == 'C'){ // C stand for 'Can you tell me how many slot is free?'
      Wire.write('C'+freeSlot);
      
    } else if(lastOrder == 'W'){ // Camera waiting for spider end of init'
      if(bInitPhotomaton) {
        Serial.println("GOOD");
        Wire.write('S');
      }
    }
    lastOrder = '0';
  }
}
void checkAvailability(){
  if(positions[0] == 1){
    Wire.write('G'); // Master can give is strip.
    bCheckAvailability = false;
    bCanProcess = false;
  }
}

void process(){
  
}
/***************************
 *    INIT
 **************************/
void initSpider(){

  // Spider
  initSpiderUp();
  initSpiderBottom();
  initSpiderUp();

  initRotate();
  // Init Servo
  setServoArmWaitPos();
  
  bInitPhotomaton = true;
}

