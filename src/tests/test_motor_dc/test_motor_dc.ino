
#include <DirectIO.h>

#define MOTOR_DIR 11 
#define MOTOR_PWM 10

void setup() { //Setup runs once//
Serial.begin(9600);
pinMode(11, OUTPUT);
pinMode(12, OUTPUT);
}

void loop() { //Loop runs forever//

delay(5000);
Serial.write("HIGH");// le HIGH monte
digitalWrite(MOTOR_DIR, HIGH);
analogWrite(MOTOR_PWM, 125);
delay(500);
analogWrite(MOTOR_PWM, 0);

delay(5000);
Serial.write("LOW");
digitalWrite(MOTOR_DIR, LOW);
analogWrite(MOTOR_PWM, 125);
delay(500);
analogWrite(MOTOR_PWM, 0);

}
