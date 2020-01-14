#include "spider.h"

// Spider rotate
AccelStepper spiderRotate(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);
Servo servoArm;
Adafruit_NeoPixel ledstrip = Adafruit_NeoPixel(84, LEDSTRIP_PIN, NEO_RGB + NEO_KHZ800);

bool bImpair = false; // if true arm is on tank 1, 3, 5, 7, 9, 11, 13
volatile byte currentSpeed = 0; // use to reset speed after a pause.

void setupSpider(){
  pinMode(SPIDER_UPDOWN_PIN_PWM, OUTPUT);
  pinMode(SPIDER_UPDOWN_PIN_DIR, OUTPUT);
  
  pinMode(SPIDER_ROTATE_PIN_ENABLE, OUTPUT);
  pinMode(SPIDER_ROTATE_ENDSTOP1_PIN, INPUT_PULLUP);
  pinMode(SPIDER_ROTATE_ENDSTOP2_PIN, INPUT_PULLUP);

  pinMode(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM, INPUT_PULLUP);
  pinMode(SPIDER_UPDOWN_PIN_ENDSTOP_UP, INPUT_PULLUP);
}

void initSpider(byte *slots){
  // TODO: Check slots state maybre?
  
  ledstrip.begin(); 
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  for(int i=0;i<LEDSTRIP_NB;i++){
    ledstrip.setPixelColor(i, 0, 0, 0); 
  }
  ledstrip.show();
  
  //initSpiderBottom();
  initServoArm();
  initSpiderUp();
  initRotate(slots);
}

void downSpider(){
  unsigned long startMoove = millis();
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  // we start fast.
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
    for(int i = SPIDER_UPDOWN_LOW_SPEED; i < SPIDER_UPDOWN_MAX_SPEED; i++){
      currentSpeed = i;
      analogWrite(SPIDER_UPDOWN_PIN_PWM, i); //max speed.
      delay(5);
    }
  }
  // and slow down after 4 seconds.
  unsigned long currentMillis = startMoove;
  while (!bEndStop) { 
    if(currentMillis - startMoove > 4000){
      currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
      analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed); // mid speed
    }
    currentMillis = millis();
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
}

void downABitSpider(){
  unsigned long startMoove = millis();
  unsigned long currentMillis = startMoove;
  
  digitalWrite(SPIDER_UPDOWN_PIN_DIR,LOW);
  currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
  analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed); //mid speed.
  
  while(currentMillis - startMoove < 500){
    // do nothing
    currentMillis = millis();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
}

void upSpider(){
  // we start fast.
  unsigned long startMoove = millis();
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, HIGH);
    for(int i = SPIDER_UPDOWN_LOW_SPEED; i < SPIDER_UPDOWN_MAX_SPEED; i++){
      analogWrite(SPIDER_UPDOWN_PIN_PWM, i); //max speed.
      currentSpeed = i;
      delay(5);
    }
  }
  // and slow down after 4 seconds.
  unsigned long currentMillis = startMoove;
  while (!bEndStop) { 
    if(currentMillis - startMoove > 4000){
      currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
      analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
    }
    currentMillis = millis();
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
}

void asyncSpiderUp() {
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR,HIGH);
    currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
    analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
}

void rotateSpider(byte *slots){
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);
  int delta = SPIDER_ROTATE_NBSTEP;
  delta += 20;
  spiderRotate.moveTo(delta);

  boolean bEndStop = false;
  while (!bEndStop) { 
    if(spiderRotate.currentPosition() > delta - 50){ // Digital read at the last time.
      bEndStop = bImpair ? !digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN) : !digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN);
    }
    spiderRotate.run();
  }
  spiderRotate.stop();
  spiderRotate.setCurrentPosition(0);
  spiderRotate.run();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  bImpair = !bImpair;

  // switch slots states.
  byte slot13 = slots[13];
  for(byte i = 13; i > 0; i--){
    slots[i] == slots[i - 1];
  }
  slots[0] = slot13;
  
  lightStrip(slots);
}

void agitate(){
  digitalWrite(SPIDER_UPDOWN_PIN_DIR, HIGH);
  currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
  analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  unsigned long startMoove = millis();
  unsigned long currentMillis = startMoove;
  while(currentMillis - startMoove < 800){
    // do nothing
    currentMillis = millis();
  }

  digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  while(!bEndStop){
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
}

void setServoArmWaitPos() {
  servoArm.write(SERVO_ARM_IDLE_POS);   
}

void openArm() {
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);// Switch rotation stepper activated to avoid rotation during operation.
  servoArm.attach(SERVO_ARM);
  servoArm.write(SERVO_ARM_OPEN_POS_BEGIN); 
  delay(500);
  for(int i = SERVO_ARM_OPEN_POS_BEGIN; i < 150; i++){
    servoArm.write(i);
    delay(10);
  }
  servoArm.detach();// To save power.
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}

void closeArm() {
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);// Switch rotation stepper activated to avoid rotation during operation.
  servoArm.attach(SERVO_ARM);
  servoArm.write(SERVO_ARM_CLOSE_POS_BEGIN); 
  delay(500);
  for(int i = SERVO_ARM_CLOSE_POS_BEGIN; i >= SERVO_ARM_CLOSE_POS_END; i--){
    servoArm.write(i);
    delay(10);
  }
  servoArm.detach();// To save power.
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}


/***************************
 *    INITS
 **************************/
void initServoArm(){
  // servo arm position
  servoArm.attach(SERVO_ARM);
  servoArm.write(0);  
  setServoArmWaitPos();
  servoArm.detach();// To save power.
}

void initSpiderBottom() {
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
}


void initSpiderUp() {
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
}

void initRotate(byte *slots) {
  // 
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);

  boolean bEndStop = !digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN);
  int homing = 0;
  while (!bEndStop) { 
    spiderRotate.moveTo(homing); 
    spiderRotate.run();
    homing++;
    delay(5);
    bEndStop = !digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN);
  }
  bImpair = true;
  spiderRotate.setCurrentPosition(0);
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);

  lightStrip(slots);
}

boolean isSpiderUp(){
  return digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
}

void lightStrip(byte *slots){
  // light open slots
  for(byte i = 0; i < 14; i++){
    for(byte j = i * 5; j < (i * 5) + 5; j++){
      ledstrip.setPixelColor(j, slots[i] == SLOT_OPEN || slots[i] == SLOT_PAPER ? 255 : 0, 0, 0); 
    }
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
