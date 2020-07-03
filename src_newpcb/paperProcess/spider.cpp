#include "spider.h"

// Spider rotate
AccelStepper spiderRotate(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);
Servo servoArm;
Adafruit_NeoPixel ledstrip = Adafruit_NeoPixel(LIGHT_PROCESS_NB, LIGHT_PROCESS_PIN, NEO_RGB + NEO_KHZ800);

bool bImpair = false; // if true arm is on tank 1, 3, 5, 7, 9, 11, 13
volatile byte currentSpeed = 0; // use to reset speed after a pause.

void setupSpider(){
  pinMode(SPIDER_UPDOWN_PIN_PWM, OUTPUT);
  pinMode(SPIDER_UPDOWN_PIN_DIR, OUTPUT);

  pinMode(SPIDER_ROTATE_PIN_M0, OUTPUT);
  digitalWrite(SPIDER_ROTATE_PIN_M0, HIGH);// Half step
  pinMode(SPIDER_ROTATE_PIN_ENABLE, OUTPUT);
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  
  pinMode(SPIDER_ROTATE_ENDSTOP1_PIN, INPUT_PULLUP);
  pinMode(SPIDER_ROTATE_ENDSTOP2_PIN, INPUT_PULLUP);
  pinMode(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM, INPUT_PULLUP);
  pinMode(SPIDER_UPDOWN_PIN_ENDSTOP_UP, INPUT_PULLUP);
  
}

void initSpider(struct storage *params){
  ledstrip.begin(); 
  for(int i=0;i<LIGHT_PROCESS_NB;i++){
    ledstrip.setPixelColor(i, 0, 0, 0); 
  }
  ledstrip.show();
  
  //If spider on top, move a bit to safely position servo.
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  if(!digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP)){
    downABitSpider();
  }
  initServoArm();
  initSpiderUp();
  initRotate(params);
}

void downSpider(){
  Serial.println("downSpider-begin");
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  unsigned long startMoove = millis();
  
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  Serial.println(bEndStop);
  // we start fast.
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
    currentSpeed = SPIDER_UPDOWN_MAX_SPEED;
    analogWrite(SPIDER_UPDOWN_PIN_PWM, SPIDER_UPDOWN_MAX_SPEED); //max speed.
  }
  // and slow down after 1 seconds.
  unsigned long currentMillis = startMoove;
  while (!bEndStop) { 
    if(currentMillis - startMoove > 900){
      currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
      analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed); // mid speed
    }
    currentMillis = millis();
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  Serial.println("downSpider-end");
}

void downABitSpider(){
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  unsigned long startMoove = millis();
  unsigned long currentMillis = startMoove;
  
  digitalWrite(SPIDER_UPDOWN_PIN_DIR,LOW);
  currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
  analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed); //mid speed.
  
  while(currentMillis - startMoove < 550){
    // do nothing
    currentMillis = millis();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}

void upSpider(byte speedPwm){
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  // we start fast.
  unsigned long startMoove = millis();
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, HIGH);
    analogWrite(SPIDER_UPDOWN_PIN_PWM, speedPwm);
  }
  // and slow down after 1 seconds.
  unsigned long currentMillis = startMoove;
  while (!bEndStop) { 
    if(currentMillis - startMoove > 1000){
      currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
      analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
    }
    currentMillis = millis();
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}

void asyncSpiderUp() {
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  if(!bEndStop){
    digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
    digitalWrite(SPIDER_UPDOWN_PIN_DIR,HIGH);
    currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
    analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  } else {
    analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
    currentSpeed = 0;
  }
}

void downToMiddleSpider(){
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  unsigned long startMoove = millis();
  unsigned long currentMillis = startMoove;
  
  digitalWrite(SPIDER_UPDOWN_PIN_DIR,LOW);
  currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
  analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  
  while(currentMillis - startMoove < 600){
    // do nothing
    currentMillis = millis();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}

void rotateSpider(struct storage *params){
  Serial.println("rotateSpider-begin");
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);
  int delta = SPIDER_ROTATE_NBSTEP;
  delta += 50;
  spiderRotate.moveTo(delta);

  boolean bEndStop = false;
  while (!bEndStop) { 
    bEndStop = bImpair ? !digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN) : !digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN);
    spiderRotate.run();
  }
  spiderRotate.stop();
  spiderRotate.setCurrentPosition(0);

  //More step to center perfectly the arm.
  spiderRotate.moveTo(SPIDER_ROTATE_CENTER_STEP); 
  while (spiderRotate.currentPosition() != SPIDER_ROTATE_CENTER_STEP) { 
    spiderRotate.run();
  }
  spiderRotate.stop();
  spiderRotate.setCurrentPosition(0);
  spiderRotate.run();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  bImpair = !bImpair;

  switchSlots(params);

  Serial.println("rotateSpider-end");
}


void blindRotate(struct storage *params){
  Serial.println("blindRotate-begin");
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);
  spiderRotate.moveTo(SPIDER_ROTATE_NBSTEP);

  while (spiderRotate.currentPosition() != SPIDER_ROTATE_NBSTEP) { 
    spiderRotate.run();
  }
  spiderRotate.stop();
  spiderRotate.setCurrentPosition(0);
  spiderRotate.run();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  bImpair = !bImpair;

  switchSlots(params);
  
  Serial.println("blindRotate-end");
}

void switchSlots(struct storage *params){
  debug("switchSolts", String("begin"));
  // switch slots states.
  byte slot13 = params->slots[13];
  for(byte i = 13; i > 0; i--){
    params->slots[i] = params->slots[i - 1];
  }
  params->slots[0] = slot13;
  debug("slots[0]",params->slots[0]);
  EEPROM.writeBlock(EEPROM_ADRESS, *params);
  lightStrip(params);
}

void agitate(){
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  digitalWrite(SPIDER_UPDOWN_PIN_DIR, HIGH);
  currentSpeed = SPIDER_UPDOWN_AGITATE_SPEED;
  analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  unsigned long startMoove = millis();
  unsigned long currentMillis = startMoove;
  //UP
  while(currentMillis - startMoove < 600){
    // do nothing
    currentMillis = millis();
  }

  //DOWN
  currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
  analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  while(!bEndStop){
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}

void setServoArmWaitPos() {
  Serial.println("setServoArmWaitPos-begin");
  servoArm.attach(SERVO_ARM); 
  servoArm.write(60);// to "activate" the crappy servo. 
  delay(500);
  servoArm.write(SERVO_ARM_IDLE_POS);   
  delay(1000);
  servoArm.detach();// To save power.
  Serial.println("setServoArmWaitPos-end");
}

void openArm(struct storage *params) {
  downABitSpider();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  servoArm.attach(SERVO_ARM);
  servoArm.write(SERVO_ARM_OPEN_POS_BEGIN); 
  delay(2000);
  upSpider(SPIDER_UPDOWN_LOW_SPEED);
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  for(int i = SERVO_ARM_OPEN_POS_BEGIN; i < SERVO_ARM_OPEN_POS_END; i++){
    servoArm.write(i);
    delay(40);
  }
  setServoArmWaitPos();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  params->slots[0] = SLOT_OPEN;
  EEPROM.writeBlock(EEPROM_ADRESS, *params);
}

void closeArm(struct storage *params) {
  downABitSpider();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);// Activate rotation stepper to avoid rotation during operation.
  servoArm.attach(SERVO_ARM);
  servoArm.write(SERVO_ARM_CLOSE_POS_BEGIN); 
  delay(1000);
  upSpider(SPIDER_UPDOWN_LOW_SPEED);
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  for(int i = SERVO_ARM_CLOSE_POS_BEGIN; i > SERVO_ARM_CLOSE_POS_END; i--){
    servoArm.write(i);
    delay(40);
  }
  servoArm.detach();// To save power.
  setServoArmWaitPos();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  params->slots[0] = SLOT_CLOSED;
  EEPROM.writeBlock(EEPROM_ADRESS, *params);
}


/***************************
 *    INITS
 **************************/
void initServoArm(){
  // servo arm position
  setServoArmWaitPos();
}

void initSpiderBottom() {
  Serial.println("initSpiderBottom");
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM); // true if not pressed.
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
    currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
    analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  }
  while (!bEndStop) { 
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}


void initSpiderUp() {
  Serial.println("initSpiderUp-begin");
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, HIGH);
    currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
    analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  }
  while (!bEndStop) { 
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  Serial.println("initSpiderUp-end");
}

void initRotate(struct storage *params) {
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
  bImpair = true;
  spiderRotate.stop();
  spiderRotate.setCurrentPosition(0);

  //More step to the center the arm.
  spiderRotate.moveTo(SPIDER_ROTATE_CENTER_STEP); 
  while (spiderRotate.currentPosition() != SPIDER_ROTATE_CENTER_STEP) { 
    spiderRotate.run();
  }
  spiderRotate.stop();
  spiderRotate.setCurrentPosition(0);
  
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  lightStrip(params);
  Serial.println("initRotate-end"); 
}

boolean isSpiderUp(){
  return !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
}

void lightStrip(struct storage *params){
  // light open slots
  // TODO: code to use when ring for led is made.
  /*for(byte i = 0; i < 14; i++){
    for(byte j = i * 5; j < (i * 5) + 5; j++){
      ledstrip.setPixelColor(j, params->slots[i] == SLOT_OPEN || params->slots[i] == SLOT_PAPER ? 255 : 0, 0, 0); 
    }
  }
  ledstrip.show();*/

  for(byte i = 0; i < 87; i++){
    ledstrip.setPixelColor(i, 255, 0, 0); 
  }
  ledstrip.show();
  
}

byte getSpiderCurrentSpeed(){
  return currentSpeed;
}

void lightFullStrip(){
  // light open slots
  for(byte i = 0; i < 84; i++){
    ledstrip.setPixelColor(i, 255, 0, 0); 
  }
  ledstrip.show();
}

 boolean bSpiderImpair(){
  return bImpair;
}


void initSlots(struct storage *params){
  // Init the slots
  params->slots[0] = SLOT_CLOSED; 
  params->slots[1] = SLOT_NO_ARM; 
  params->slots[2] = SLOT_CLOSED; 
  params->slots[3] = SLOT_NO_ARM; 
  params->slots[4] = SLOT_CLOSED; 
  params->slots[5] = SLOT_NO_ARM; 
  params->slots[6] = SLOT_CLOSED; 
  params->slots[7] = SLOT_NO_ARM; 
  params->slots[8] = SLOT_CLOSED; 
  params->slots[9] = SLOT_NO_ARM; 
  params->slots[10] = SLOT_CLOSED; 
  params->slots[11] = SLOT_NO_ARM; 
  params->slots[12] = SLOT_CLOSED;
  params->slots[13] = SLOT_NO_ARM;

  EEPROM.writeBlock(EEPROM_ADRESS, *params);
}
