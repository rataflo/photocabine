#include <Servo.h>
#define SERVO_ARM 46
Servo servoArm;
int pos = 0;
void setup() {
  // put your setup code here, to run once:
// servo arm position
  servoArm.attach(SERVO_ARM);
  servoArm.write(70);  
}

void loop() {
  // put your main code here, to run repeatedly:
/*for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoArm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servoArm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }*/
}
