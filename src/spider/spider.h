
// Spider up/down
Input<SPIDER_UPDOWN_BOTTOM_ENDSTOP_PIN> endstopSpiderBottom;
Input<SPIDER_UPDOWN_UPPER_ENDSTOP_PIN> endstopSpiderUp;
Output<SPIDER_UPDOWN_PIN_DOWN> spiderDownPin;
Output<SPIDER_UPDOWN_PIN_DOWN> spiderUpPin;

// Spider rotate
AccelStepper spiderRotate(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);
Input<SPIDER_ROTATE_ENDSTOP1_PIN> endstop1Rotate;
Input<SPIDER_ROTATE_ENDSTOP2_PIN> endstop2Rotate;

void initSpiderBottom() {
  boolean bEndStop = endstopSpiderBottom.read();
  if(!bEndStop){
    spiderDownPin.write(LOW);
  }
  while (!bEndStop) { 
    bEndStop = endstopSpiderBottom.read();
  }
  timeToGoDown = millis() - timeToGoDown;
  timeToGoUp = millis();
  spiderDownPin.write(HIGH);
}


void initSpiderUp() {
  boolean bEndStop = endstopSpiderUp.read();
  if(!bEndStop){
    spiderUpPin.write(LOW);
  }
  while (!bEndStop) { 
    bEndStop = endstopSpiderUp.read();
  }
  timeToGoUp = millis() - timeToGoUp;
  spiderUpPin.write(HIGH);
  timeToGoDown = millis();
}

void initRotate() {
  boolean bEndStop = !endstop1Rotate.read();
  while (!bEndStop) { 
    spiderRotate.moveTo(spiderRotate.currentPosition() - 1); 
    spiderRotate.run();
    bEndStop = !endstop1Rotate.read();
  }
  spiderRotate.setCurrentPosition(0);
  arm0Pos = 0;
}

void downSpider(){
}

void upSpider(){
}

void rotateSpider(){
  spiderRotate.moveTo(spiderRotate.currentPosition() - SPIDER_ROTATE_NBSTEP);
}





