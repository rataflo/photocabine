/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 */
#include <Wire.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include "constants.h"

/*
 * GLOBAL VARIABLES
 */
 
// Spider up/down



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
static volatile byte bSlotReady = LOW;
//static volatile bool bBottom = HIGH;
bool bArmOpen = false;

// timer
unsigned long currentMillis = 0;
unsigned long startWaitStrip = 0;

#include "servoArm.h"
#include "tests.h"

void setup() {
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

  pinMode(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM, INPUT_PULLUP);
  pinMode(SPIDER_UPDOWN_PIN_ENDSTOP_UP, INPUT_PULLUP);
  pinMode(SPIDER_UPDOWN_PIN_DIR, OUTPUT);
  
  initSpider();
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
}

void respondToOrder(){
  if(lastOrder != '0') {
    if(lastOrder == 'G') { // G stand for 'Give me a place for processing my strip'.
      needAPlace = true;
      Wire.write('G'); 
      lastOrder = '0';
           
    }if(lastOrder == 'K' && bSlotReady == HIGH) { // K stand for Kangourou. Or, the slot is ready?
      //Serial.print('L');
      Wire.write('K'); 
      bSlotReady = LOW;
      startWaitStrip = millis();
      lastOrder = '0';
           
    } else if(lastOrder == 'O'){ // O stand for 'Oooo yeah i finished to give you the strip, do your job!'
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
      // Pair = 14 (13 juste aprés rotation)
      if(!bImpair){
        // SI besoin sortie papier
        for(int i = 0; i < 7; i++){
          if(arms[i] == 13){
            exitPaper();
            arms[i] = -1;
            break;
          } 
        }

        // Check si fin du process.
        bool bEndProcess = true;
        for(int i = 0; i < 7; i++){
          if(arms[i] != -1){
            bEndProcess = false;
          } 
        }
        if(bEndProcess){
          stepSpider = 255;
        }
        
        // Si besoin place
        if(needAPlace){
          // descend d'un poil
          digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
          analogWrite(SPIDER_UPDOWN_PIN_PWM, 255); //max speed.
          delay(500);
          analogWrite(SPIDER_UPDOWN_PIN_PWM, 0); //max speed.
          delay(1000);
          setServoArmOpenPos();

          initSpiderUp();
          delay(1000);
          openArm();
          bArmOpen = true;
        }
      } else { // Impair = 1
        if(needAPlace && bArmOpen){
          bArmOpen = false;
          needAPlace = false;
          bSlotReady = true;
        }
      }

      if(stepSpider != 255){
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
}

void openNewPlace(){
  
  if(needAPlace && stepSpider == 255 && bSpiderUp){
    //noInterrupts();
    if(!bImpair) {
      digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
      analogWrite(SPIDER_UPDOWN_PIN_PWM, 255); //max speed.
      delay(500);
      analogWrite(SPIDER_UPDOWN_PIN_PWM, 0); //max speed.
      delay(1000);
      setServoArmOpenPos();

      spiderRotate.setCurrentPosition(0);
      spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
      spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);
      int delta = -20;
      spiderRotate.moveTo(delta);

      while (spiderRotate.currentPosition() != delta){ // Digital read at the last time.
        spiderRotate.run();
      }

      delay(1000);
      initSpiderUp();

      spiderRotate.setCurrentPosition(0);
      spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
      spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);
      delta = 20;
      spiderRotate.moveTo(delta);

      while (spiderRotate.currentPosition() != delta){ // Digital read at the last time.
        spiderRotate.run();
      }
  
      
      delay(1000);
      openArm();
      //noInterrupts();
      rotateSpider();
      //interrupts();

      needAPlace = false;
      bSlotReady = HIGH;
      setServoArmWaitPos();
      
    } else {
      setServoArmOpenPos();
      rotateSpider();
      openArm();
      rotateSpider();
      needAPlace = false;
      bSlotReady = HIGH;
      setServoArmWaitPos();
    }
    //interrupts();
  }
  
}

/******************************************************************************
 * SPIDER
 ******************************************************************************/

void downSpider(){
  unsigned long startMoove = millis();
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
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
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  bSpiderUp = false;
}

void upSpider(){
  unsigned long startMoove = millis();
  
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR,HIGH);
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
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
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
  digitalWrite(SPIDER_UPDOWN_PIN_DIR,HIGH);
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 250); //mid speed.
  unsigned long startMoove = millis();
  currentMillis = startMoove;
  while(currentMillis - startMoove < 800){
    // do nothing
    currentMillis = millis();
  }

  digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
  //analogWrite(SPIDER_UPDOWN_PIN_PWM, 125); //mid speed.
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  while(!bEndStop){
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
}

void exitPaper(){
  //On descend un chouilla.
  downSpider();
    
  servoExit.write(80);
  exitCatchPosition();
  //on remonte
  upSpider();
  servoExit.write(0);
  delay(1000);
  // on redescend.
  downSpider();
  
  initExit();
  servoExit.write(80);
  
  setServoArmClosePos();
  //on remonte
  initSpiderUp();
  delay(1000);
  closeArm();
  delay(1000);
  setServoArmWaitPos();
  // fermeture du bras.
  freeSlot++;
}

/***************************
 *    INIT
 **************************/
void initSpider(){

  // Init Servo
  setServoArmWaitPos();

  //initExit();
  
  //spiderBottom();*/
  initSpiderUp();

  initRotate();

  servoExit.write(80);
  /*delay(5000);
  servoExit.write(80);*/
  
  bInitSpider = true;
}

void initSpiderBottom() {
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
    analogWrite(SPIDER_UPDOWN_PIN_PWM, 125); //max speed.
  }
  while (!bEndStop) { 
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  bSpiderUp = false;
}


void initSpiderUp() {
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR,HIGH);
    analogWrite(SPIDER_UPDOWN_PIN_PWM, 125); //max speed.
  }
  while (!bEndStop) { 
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
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
  bImpair = false;
  spiderRotate.setCurrentPosition(0);
}

void initExit() {
  stepperExit.setCurrentPosition(0);
  stepperExit.setMaxSpeed(100);
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
  stepperExit.setMaxSpeed(100);
  stepperExit.setAcceleration(100);

  int delta = 428;
  stepperExit.moveTo(delta); 

  while(stepperExit.currentPosition()< delta){
    stepperExit.run();
  }
}
