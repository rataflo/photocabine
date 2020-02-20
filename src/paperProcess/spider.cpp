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
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
}

void initSpider(struct storage *params){
  ledstrip.begin(); 
  for(int i=0;i<LEDSTRIP_NB;i++){
    ledstrip.setPixelColor(i, 0, 0, 0); 
  }
  ledstrip.show();
  
  //initSpiderBottom();
  // TODO: descendre un petit peu avant de bouger le servo.
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
    for(int i = SPIDER_UPDOWN_LOW_SPEED; i <= SPIDER_UPDOWN_MAX_SPEED; i++){
      currentSpeed = i;
      analogWrite(SPIDER_UPDOWN_PIN_PWM, i); //max speed.
      delay(5);
    }
  }
  // and slow down after 4 seconds.
  unsigned long currentMillis = startMoove;
  while (!bEndStop) { 
    if(currentMillis - startMoove > 4200){
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
  
  while(currentMillis - startMoove < 1000){
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
    for(int i = SPIDER_UPDOWN_LOW_SPEED; i <= speedPwm; i++){
      analogWrite(SPIDER_UPDOWN_PIN_PWM, i);
      currentSpeed = i;
      delay(5);
    }
  }
  // and slow down after 4 seconds.
  unsigned long currentMillis = startMoove;
  while (!bEndStop) { 
    if(currentMillis - startMoove > 4200){
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
  analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed); //mid speed.
  
  while(currentMillis - startMoove < 1500){
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
  delta += 200;
  spiderRotate.moveTo(delta);

  boolean bEndStop = false;
  while (!bEndStop) { 
    bEndStop = bImpair ? !digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN) : !digitalRead(SPIDER_ROTATE_ENDSTOP2_PIN);
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

  while (spiderRotate.currentPosition() < SPIDER_ROTATE_NBSTEP) { 
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
  currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
  analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed);
  unsigned long startMoove = millis();
  unsigned long currentMillis = startMoove;
  //UP
  while(currentMillis - startMoove < 800){
    // do nothing
    currentMillis = millis();
  }

  //DOWN
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
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);// Activate rotation stepper to avoid rotation during operation.
  downABitSpider();
  servoArm.attach(SERVO_ARM);
  servoArm.write(SERVO_ARM_OPEN_POS_BEGIN); 
  delay(2000);
  upSpider(SPIDER_UPDOWN_LOW_SPEED);
  for(int i = SERVO_ARM_OPEN_POS_BEGIN; i < SERVO_ARM_OPEN_POS_END; i++){
    servoArm.write(i);
    delay(40);
  }
  servoArm.detach();// To save power.
  setServoArmWaitPos();
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
  params->slots[0] = SLOT_OPEN;
  EEPROM.writeBlock(EEPROM_ADRESS, *params);
}

void closeArm(struct storage *params) {
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);// Switch rotation stepper activated to avoid rotation during operation.
  downABitSpider();
  servoArm.attach(SERVO_ARM);
  servoArm.write(SERVO_ARM_CLOSE_POS_BEGIN); 
  delay(1000);
  upSpider(SPIDER_UPDOWN_LOW_SPEED);
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
  lightStrip(params);
  Serial.println("initRotate-end"); 
}

boolean isSpiderUp(){
  return !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_UP);
}

void lightStrip(struct storage *params){
  // light open slots
  for(byte i = 0; i < 14; i++){
    for(byte j = i * 5; j < (i * 5) + 5; j++){
      ledstrip.setPixelColor(j, params->slots[i] == SLOT_OPEN || params->slots[i] == SLOT_PAPER ? 255 : 0, 0, 0); 
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
