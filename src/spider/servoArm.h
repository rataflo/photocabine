// servo arm position
Servo servoArm;
Servo servoExit;

void setServoArmWaitPos() {
  servoArm.write(65);  
}

void setServoArmOpenPos() {
  servoArm.write(0);  
}

void setServoArmClosePos() {
  servoArm.write(190);  
}

void openArm() {
  for(int i = 0; i < 150; i++){
    servoArm.write(i);
    delay(10);
  }
  
  // retour arriére spider pour compenser le mouvement induit par le servo.
  /*spiderRotate.setCurrentPosition(0);
  spiderRotate.setMaxSpeed(SPIDER_ROTATE_SPEED);
  spiderRotate.setAcceleration(SPIDER_ROTATE_ACCEL);

  boolean bEndStop = digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN);
  int homing = 0;
  while (!bEndStop) { 
    spiderRotate.moveTo(homing); 
    spiderRotate.run();
    homing--;
    delay(5);
    bEndStop = digitalRead(SPIDER_ROTATE_ENDSTOP1_PIN);
  }
  spiderRotate.setCurrentPosition(0);*/
}

void closeArm() {
  for(int i = 190; i >= 20; i--){
    servoArm.write(i);
    delay(10);
  }
}
