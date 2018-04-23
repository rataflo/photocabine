/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 */
#include <Wire.h>
#include <DirectIO.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include "constants.h"

/*
 * GLOBAL VARIABLES
 */
 
// Spider up/down
Input<SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM> endstopSpiderBottom(true);
Input<SPIDER_UPDOWN_PIN_ENDSTOP_UP> endstopSpiderUp(true);
Output<SPIDER_UPDOWN_PIN_DIR> spiderDir;


// Spider rotate
AccelStepper spiderRotate(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);
Input<SPIDER_ROTATE_ENDSTOP1_PIN> endstop1Rotate(true);
Input<SPIDER_ROTATE_ENDSTOP2_PIN> endstop2Rotate(true);

int arms[7] = {-1, -1, -1, -1, -1, -1, -1}; // State of arms (-1 = nothing, >= 0 step of arm)
byte freeSlot = 7; // number of free arms.

// Work variables
bool bInitSpider = false;
bool bSpiderUp = true; // true when spider is at the top.
static volatile char lastOrder = '0';
int stepSpider = -1;
bool bImpair = false; // if true arm is on tank 1, 3, 5, 7, 9, 11, 13
bool needAPlace = false;
bool bSlotReady = false;

// time to go from bottom to upper switch.
unsigned long timeToGoUp = 0;
// time to go from upper switch to bottom switch.
unsigned long timeToGoDown = 0;

// timer
unsigned long currentMillis = 0;

#include "servoArm.h"
#include "tests.h"

void setup() {
  Serial.begin(9600);
  Wire.begin(7);
  Wire.onReceive(receiveOrder); 
  Wire.onRequest(respondToOrder); 
  
  // servo arm position
  servoArm.attach(SERVO_ARM);
  servoArm.write(35);  

  // pwm dc up and down (do not use DirectIO)
  pinMode(SPIDER_UPDOWN_PIN_PWM, OUTPUT);
  
  initSpider();
}

void loop() {
  currentMillis = millis();

  openNewPlace();
  
  if(stepSpider != -1){
    process();
  }
}

/*
 * Check if master arduino send an order. Mainly to process a strip.
 */
void receiveOrder(int lng){
  if(Wire.available() > 0) {
    lastOrder = Wire.read();
    Serial.print(lastOrder);
  }
}

void respondToOrder(){
  if(lastOrder != '0') {
    if(lastOrder == 'G') { // G stand for 'Give me a place for processing my strip'.
      needAPlace = true;
      Wire.write('G'); 
           
    }if(lastOrder == 'K') { // K stand for Kangourou. Or, the slot is ready?
     if(bSlotReady){
      Serial.print('K');
      Wire.write('K'); 
      bSlotReady = false;
     }
           
    } else if(lastOrder == 'O'){ // O stand for 'Oooo yeah i finished to give you the strip, do your job!'
      Wire.write('O'); 
      for(int i = 0; i < 7; i++){
        if(arms[i] == -1){
          arms[i] = 0;
          break;
        } 
      }
      stepSpider = 0; 
      freeSlot--;
      
    } else if(lastOrder == 'C'){ // C stand for 'Can you tell me how many slot is free?'
      Wire.write('C'+freeSlot);
      
    } else if(lastOrder == 'W' && bInitSpider){ // Camera waiting for spider end of init'
      Wire.write('S');
    } 
    
    lastOrder = '0';
  }
}

void process(){
  while (stepSpider != -1 && stepSpider < 14) {
      downSpider();
      
      // Wait for process and agitate.
      unsigned long startMillis = millis();
      currentMillis = startMillis;
      while(currentMillis - startMillis < TANK_TIME){
        agitate();
        currentMillis = millis();
      }
      
      upSpider();

      delay(4000);

      stepSpider++;
      Serial.println(stepSpider);
      
      // Move each arm 1 step.
      for(int i = 0; i < 7; i++){
        if(arms[i] != -1){
          arms[i]++;
        }
      }
        
      // Check if arm positionned on exit contain paper
      for(int i = 0; i < 7; i++){
        if(!bImpair && arms[i] == 14){
          exitPaper();
        } 
      }

      bool needClose = false;
      if(!bImpair && needAPlace){
        setServoArmOpenPos();
      } else {
        // check si un bras dois être fermé.
        /*for(int i = 0; i < 7; i++){
          if(arms[i] == 14){
            setServoArmClosePos();
            needClose = true;
            break;
          }
        }*/
      }
      
      // Rotate until end of process or if a place is needed or a close of arm.
      //if(stepSpider < 14 || needAPlace || needClose){
      if(stepSpider < 14 || needAPlace){
        rotateSpider();
      } else {
        stepSpider = -1;
      }
      
      // If arm open need.
      if(bImpair && needAPlace){
        openArm();
        //setServoArmWaitPos();
        stepSpider = -1; // no action until paper received.
        needAPlace = false;
        bSlotReady = true;
        for(int i = 0; i < 7; i++){
          if(arms[i] == 14){
            arms[i] = -1;
            break;
          }
        }
      }
      // If arm close need.
      /*if(bImpair && needClose && !needAPlace){
        Serial.println("close");
        closeArm();
        needClose = false;
        for(int i = 0; i < 7; i++){
          if(arms[i] == 14){
            arms[i] = -1;
            break;
          }
        }
      }*/
  }
}

void openNewPlace(){
  if(needAPlace && stepSpider == -1 && !bImpair && bSpiderUp) {
    setServoArmOpenPos();
    rotateSpider();
    openArm();
    //setServoArmWaitPos();
    needAPlace = false;
    bSlotReady = true;
  }
}

/******************************************************************************
 * SPIDER
 ******************************************************************************/

void downSpider(){
  unsigned long startMoove = millis();
  boolean bEndStop = !endstopSpiderBottom.read();
  if(!bEndStop){
    spiderDir.write(LOW);
    for(int i = 50; i < 255; i++){
      analogWrite(SPIDER_UPDOWN_PIN_PWM, i); //max speed.
      delay(5);
    }
  }

  while (!bEndStop) { 
    currentMillis = millis();
    if(currentMillis - startMoove > 4000){
      analogWrite(SPIDER_UPDOWN_PIN_PWM, 125);
    }
    bEndStop = !endstopSpiderBottom.read();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  bSpiderUp = false;
}

void upSpider(){
  unsigned long startMoove = millis();
  
  boolean bEndStop = !endstopSpiderUp.read();
  if(!bEndStop){
    spiderDir.write(HIGH);
    for(int i = 50; i < 255; i++){
      analogWrite(SPIDER_UPDOWN_PIN_PWM, i); //max speed.
      delay(5);
    }
  }
  while (!bEndStop) { 
    currentMillis = millis();
    if(currentMillis - startMoove > 4000){
      analogWrite(SPIDER_UPDOWN_PIN_PWM, 125);
    }
    bEndStop = !endstopSpiderUp.read();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  bSpiderUp = true;
}

void rotateSpider(){
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);
  int delta = SPIDER_ROTATE_NBSTEP;
  delta += 20;
  spiderRotate.moveTo(delta);

  boolean bEndStop = false;
  while (!bEndStop) { 
    if(spiderRotate.currentPosition() > delta - 50){ // Digital read at the last time.
      bEndStop = bImpair ? endstop2Rotate.read() : endstop1Rotate.read();
    }
    spiderRotate.run();
  }
  spiderRotate.stop();
  spiderRotate.setCurrentPosition(0);
  spiderRotate.run();
  
  bImpair = !bImpair;
}

void agitate(){
  spiderDir.write(HIGH);
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 125); //mid speed.
  delay(500);

  spiderDir.write(LOW);
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 125); //mid speed.
  boolean bEndStop = !endstopSpiderBottom.read();
  while(!bEndStop){
    bEndStop = !endstopSpiderBottom.read();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
}

void exitPaper(){
  delay(5000);
  
  freeSlot++;
}

/***************************
 *    INIT
 **************************/
void initSpider(){

  // Init Servo
  setServoArmWaitPos();
  
  // Spider
  /*initSpiderUp();
  initSpiderBottom();*/
  initSpiderUp();

  initRotate();
  
  bInitSpider = true;
}

void initSpiderBottom() {
  boolean bEndStop = !endstopSpiderBottom.read();
  if(!bEndStop){
    spiderDir.write(LOW);
    analogWrite(SPIDER_UPDOWN_PIN_PWM, 125); //max speed.
  }
  while (!bEndStop) { 
    bEndStop = !endstopSpiderBottom.read();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  bSpiderUp = false;
}


void initSpiderUp() {
  boolean bEndStop = !endstopSpiderUp.read();
  if(!bEndStop){
    spiderDir.write(HIGH);
    analogWrite(SPIDER_UPDOWN_PIN_PWM, 125); //max speed.
  }
  while (!bEndStop) { 
    bEndStop = !endstopSpiderUp.read();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  bSpiderUp = true;
}

void initRotate() {
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);

  boolean bEndStop = endstop2Rotate.read();
  int homing = 0;
  while (!bEndStop) { 
    spiderRotate.moveTo(homing); 
    spiderRotate.run();
    homing++;
    delay(5);
    bEndStop = endstop2Rotate.read();
  }
  bImpair = false;
  spiderRotate.setCurrentPosition(0);
}

