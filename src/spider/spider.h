
// Spider up/down
Input<SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM> endstopSpiderBottom(true);
Input<SPIDER_UPDOWN_PIN_ENDSTOP_UP> endstopSpiderUp(true);
Output<SPIDER_UPDOWN_PIN_DIR> spiderDir;


// Spider rotate
AccelStepper spiderRotate(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);
Input<SPIDER_ROTATE_ENDSTOP1_PIN> endstop1Rotate(true);
Input<SPIDER_ROTATE_ENDSTOP2_PIN> endstop2Rotate(true);

void initSpiderBottom() {
  Serial.println("initSpiderBottom - debut");
  boolean bEndStop = !endstopSpiderBottom.read();
  Serial.println(bEndStop);
  if(!bEndStop){
    spiderDir.write(LOW);
    analogWrite(SPIDER_UPDOWN_PIN_PWM, 255); //mid speed.
  }
  Serial.println(bEndStop);
  Serial.println("B");
  while (!bEndStop) { 
    bEndStop = !endstopSpiderBottom.read();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  Serial.println("initSpiderBottom - fin");
}


void initSpiderUp() {
  Serial.println("initSpiderUp - debut");
  boolean bEndStop = !endstopSpiderUp.read();
  if(!bEndStop){
    spiderDir.write(HIGH);
    analogWrite(SPIDER_UPDOWN_PIN_PWM, 255); //mid speed.
  }
  while (!bEndStop) { 
    bEndStop = !endstopSpiderUp.read();
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  Serial.println("initSpiderUp - fin");
}

void initRotate() {
  // stepper spider rotation
  spiderRotate.setMaxSpeed(1000);
  spiderRotate.setAcceleration(500);

  boolean bEndStop = endstop1Rotate.read();
  int homing = 0;
  while (!bEndStop) { 
    spiderRotate.moveTo(homing); 
    spiderRotate.run();
    homing++;
    delay(5);
    bEndStop = endstop1Rotate.read();
  }
  spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(1000);
  spiderRotate.setAcceleration(500);
}

void downSpider(){
}

void upSpider(){
}

void rotateSpider(){
  spiderRotate.moveTo(spiderRotate.currentPosition() + SPIDER_ROTATE_NBSTEP);
}





