// servo arm position
Servo servoArm;


void setServoArmWaitPos() {
  servoArm.write(SERVO_ARM_IDLE_POS);  
}

void setServoArmOpenPos() {
  servoArm.write(SERVO_ARM_OPEN_POS);  
}

void setServoArmClosePos() {
  servoArm.write(SERVO_ARM_CLOSE_POS);  
}

void openArm() {
  servoArm.write(SERVO_ARM_OPEN_POS + 150); 
}

void closeArm() {
  servoArm.write(SERVO_ARM_CLOSE_POS - 150); 
}
