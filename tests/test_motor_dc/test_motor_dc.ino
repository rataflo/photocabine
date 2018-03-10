#define MOTOR_IN1 44 //CW is defined as pin #7//

#define MOTOR_IN2 45 //CCW is defined as pin #8//

void setup() { //Setup runs once//

pinMode(MOTOR_IN1, OUTPUT); //Set CW as an output//
pinMode(MOTOR_IN2, OUTPUT); //Set CCW as an output//
digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
}

void loop() { //Loop runs forever//

// ramp up forward
  
  

 delay(1000);
  // forward full speed for one second
  /*delay(1000);
  
  // ramp down forward
  for (int i=255; i>=0; i--) {
    analogWrite(MOTOR_IN2, i);
    delay(10);
  }
 
  // ramp up backward
  digitalWrite(MOTOR_IN2, LOW);
  for (int i=0; i<255; i++) {
    analogWrite(MOTOR_IN1, i);
    delay(10);
  }
 
  // backward full speed for one second
  delay(1000);
 
  // ramp down backward
  for (int i=255; i>=0; i--) {
    analogWrite(MOTOR_IN1, i);
    delay(10);
  }*/
}
