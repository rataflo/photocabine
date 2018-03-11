
#include <DirectIO.h>

#define MOTOR_IN1 44 //CW is defined as pin #7//
#define MOTOR_IN2 45 //CCW is defined as pin #8//
#define SPIDER_UPDOWN_ENDSTOP_PIN 48
Input<SPIDER_UPDOWN_ENDSTOP_PIN> switchSpider;

void setup() { //Setup runs once//
Serial.begin(9600);
pinMode(MOTOR_IN1, OUTPUT); //Set CW as an output//
pinMode(MOTOR_IN2, OUTPUT); //Set CCW as an output//
digitalWrite(MOTOR_IN1, HIGH);
digitalWrite(MOTOR_IN2, LOW);

}

void loop() { //Loop runs forever//

Serial.println(switchSpider.read());
  if(!switchSpider.read()){
    digitalWrite(MOTOR_IN2, HIGH);

    // on attend 200ms avant d'aller dans l'autre sens.
    delay(200);
    digitalWrite(MOTOR_IN1, LOW);
    // 1 seconde de montée
    delay(3700);
    digitalWrite(MOTOR_IN1, HIGH);
    // on attend 200ms avant d'aller dans l'autre sens.
    delay(200);
    digitalWrite(MOTOR_IN2, LOW);
  }
}
