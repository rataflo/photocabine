#define MOTOR_DIR 11 
#define MOTOR_PWM 3

void setup() { //Setup runs once//
Serial.begin(9600);
pinMode(11, OUTPUT);
pinMode(3, OUTPUT);
}

void loop() { //Loop runs forever//

delay(3000);
Serial.write("HIGH");// le HIGH monte
digitalWrite(MOTOR_DIR, HIGH);
analogWrite(MOTOR_PWM, 95);
delay(3000);
analogWrite(MOTOR_PWM, 0);

delay(000);
Serial.write("LOW");
digitalWrite(MOTOR_DIR, LOW);
analogWrite(MOTOR_PWM, 90);
delay(3000);
analogWrite(MOTOR_PWM, 0);

}
