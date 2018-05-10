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
AccelStepper stepperExit(1, SPIDER_EXIT_PIN_STP, SPIDER_EXIT_PIN_DIR);
//Input<SPIDER_ROTATE_ENDSTOP1_PIN> SPIDER_ROTATE_ENDSTOP1_PIN(true);
//Input<SPIDER_ROTATE_ENDSTOP2_PIN> SPIDER_ROTATE_ENDSTOP2_PIN(true);

int arms[7] = {-1, -1, -1, -1, -1, -1, -1}; // State of arms (-1 = nothing, >= 0 step of arm)
byte freeSlot = 7; // number of free arms.

// Work variables
bool bInitSpider = false;
bool bSpiderUp = true; // true when spider is at the top.
bool bImpair = false; // if true arm is on tank 1, 3, 5, 7, 9, 11, 13
volatile char lastOrder = '0';
volatile byte stepSpider = 255; // 255 mean no action (we do not use int because it take much time to load from RAM (volatile) than register).
volatile bool needAPlace = false;
volatile bool bSlotReady = false;

// timer
unsigned long currentMillis = 0;
unsigned long startWaitStrip = 0;

#include "servoArm.h"
#include "tests.h"

void setup() {
  Serial.begin(9600);
  Wire.begin(7);
  Wire.onReceive(receiveOrder); 
  Wire.onRequest(respondToOrder); 
  
  // servo arm position
  servoArm.attach(SERVO_ARM);
  servoArm.write(0);  

  // servo arm position
  servoExit.attach(SERVO_EXIT);
  servoExit.write(35);  

  // pwm dc up and down (do not use DirectIO)
  pinMode(SPIDER_UPDOWN_PIN_PWM, OUTPUT);

  pinMode(SPIDER_ROTATE_ENDSTOP1_PIN, INPUT_PULLUP);
  pinMode(SPIDER_ROTATE_ENDSTOP2_PIN, INPUT_PULLUP);
  pinMode(SWITCH_EXIT, INPUT_PULLUP);
  
  initSpider();
  Serial.println("OK");
}

void loop() {
  currentMillis = millis();

  openNewPlace();

  waitForStrip();
  
  if(stepSpider != 255){
    process();
  }
}

void waitForStrip(){
  if(startWaitStrip > 0){
    startWaitStrip = millis();
    currentMillis = startWaitStrip;
    while(currentMillis - startWaitStrip < 30000){
      currentMillis = millis(); // do nothing
    }
    startWaitStrip = 0;
    stepSpider = 0;

    for(int i = 0; i < 7; i++){
      if(arms[i] == -1){
        arms[i] = 0;
        break;
      } 
    }
  }
  
}

/*
 * Check if master arduino send an order. Mainly to process a strip.
 */
void receiveOrder(int lng){
  if(Wire.available() > 0) {
    lastOrder = Wire.read();
  }
  Serial.print('A');
}

void respondToOrder(){
  Serial.print(lastOrder);
  if(lastOrder != '0') {
    if(lastOrder == 'G') { // G stand for 'Give me a place for processing my strip'.
      needAPlace = true;
      Wire.write('G'); 
      lastOrder = '0';
           
    }if(lastOrder == 'K' && bSlotReady) { // K stand for Kangourou. Or, the slot is ready?
      Wire.write('K'); 
      bSlotReady = false;
      startWaitStrip = millis();
      lastOrder = '0';
           
    } else if(lastOrder == 'O'){ // O stand for 'Oooo yeah i finished to give you the strip, do your job!'
      Serial.print('C');
      Wire.write('O'); 
      stepSpider = 0; 
      lastOrder = '0';
      
    } else if(lastOrder == 'C'){ // C stand for 'Can you tell me how many slot is free?'
      Wire.write('C'+freeSlot);
      lastOrder = '0';
      
    } else if(lastOrder == 'W' && bInitSpider){ // Camera waiting for spider end of init'
      Wire.write('S');
      lastOrder = '0';
    } 
  }
}

void process(){
  while (stepSpider != 255) {
      // Pair = 14
      if(!bImpair){
        // SI besoin sortie papier
        for(int i = 0; i < 7; i++){
          if(arms[i] == 14){
            exitPaper();
            arms[i] = -1;
          } 
        }

        // Check si fin du process.
        bool bEndProcess = true;
        for(int i = 0; i < 7; i++){
          if(arms[i] > -1){
            bEndProcess = false;
          } 
        }
        if(bEndProcess){
          stepSpider = 255;
        }
        
        // Si besoin place
        if(needAPlace){
          // descend d'un poil
          spiderDir.write(LOW);
          analogWrite(SPIDER_UPDOWN_PIN_PWM, 255); //max speed.
          delay(100);
          analogWrite(SPIDER_UPDOWN_PIN_PWM, 0); //max speed.
          
          setServoArmOpenPos();

          initSpiderUp();
          
          openArm();
        }
      } else { // Impair = 1
        if(needAPlace){
          needAPlace = false;
          bSlotReady = true;
        }
      }
     
      downSpider();
      
      // Wait for process and agitate.
      unsigned long startMillis = millis();
      currentMillis = startMillis;
      int temps = TANK_TIME;
      while(currentMillis - startMillis < temps){
        agitate();
        currentMillis = millis();
      }
      
      upSpider();

      rotateSpider();

      stepSpider++;
      
      // Move each arm 1 step.
      for(int i = 0; i < 7; i++){
        if(arms[i] != -1){
          arms[i]++;
        }
      }
  }
}

void openNewPlace(){
  
  if(needAPlace && stepSpider == 255 && bSpiderUp){
    setServoArmOpenPos();
    if(!bImpair) {
      setServoArmOpenPos();
      openArm();
      rotateSpider();
    } else {
      rotateSpider();
      openArm();
      rotateSpider();
      needAPlace = false;
      bSlotReady = true;
    }
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
      bEndStop = bImpair ? digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN) : digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN);
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
  unsigned long startMoove = millis();
  currentMillis = startMoove;
  while(currentMillis - startMoove < 400){
    // do nothing
    currentMillis = millis();
  }

  spiderDir.write(LOW);
  //analogWrite(SPIDER_UPDOWN_PIN_PWM, 125); //mid speed.
  boolean bEndStop = !endstopSpiderBottom.read();
  while(!bEndStop){
    bEndStop = !endstopSpiderBottom.read();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
}

void exitPaper(){
  servoExit.write(80);
  exitCatchPosition();
  servoExit.write(0);
  initExit();
  servoExit.write(80);
  freeSlot++;
}

/***************************
 *    INIT
 **************************/
void initSpider(){

  // Init Servo
  setServoArmOpenPos();

  initExit();
  
  // Spider
  /*initSpiderUp();
  initSpiderBottom();*/
  initSpiderUp();

  initRotate();

  servoExit.write(80);
  /*delay(5000);
  servoExit.write(80);*/
  
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

  boolean bEndStop = digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN);
  int homing = 0;
  while (!bEndStop) { 
    spiderRotate.moveTo(homing); 
    spiderRotate.run();
    homing++;
    delay(5);
    bEndStop = digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN);
  }
  bImpair = true;
  spiderRotate.setCurrentPosition(0);
}

void initExit() {
  stepperExit.setCurrentPosition(0);
  stepperExit.setMaxSpeed(200);
  stepperExit.setAcceleration(100);

  boolean bEndStop = !digitalRead(SWITCH_EXIT);
  int homing = 0;
  while (!bEndStop) { 
    stepperExit.moveTo(homing); 
    stepperExit.run();
    homing--;
    delay(5);
    bEndStop = !digitalRead(SWITCH_EXIT);
  }
  stepperExit.setCurrentPosition(0);
}

void exitCatchPosition() {
  stepperExit.setCurrentPosition(0);
  stepperExit.setMaxSpeed(200);
  stepperExit.setAcceleration(100);

  int delta = 200;
  stepperExit.moveTo(delta); 

  while(stepperExit.currentPosition()< delta){
    stepperExit.run();
  }
  
}
