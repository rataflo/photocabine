#include <Servo.h>
#define SERVO_ARM_OPEN 6
#define SERVO_ARM_CLOSE 9
Servo servoOpen;
Servo servoClose;
int pos = 0;
void setup() {
  // put your setup code here, to run once:
// servo arm position
  servoOpen.attach(SERVO_ARM_OPEN);
  servoOpen.write(0);  
  servoClose.attach(SERVO_ARM_CLOSE);
  servoClose.write(0);  
}

void loop() {
  //positions test
  //servoOpen.write(10);
  servoClose.write(180);

  for (pos = 10; pos <= 120; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoOpen.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 120; pos >= 10; pos -= 1) { // goes from 180 degrees to 0 degrees
    servoOpen.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  // Opern & close degrees:
  Open: 10=>120
  Close: 180 => 40?
  Rest: 180. Nécessite d'être positionné à 10 avant rotation.
}
