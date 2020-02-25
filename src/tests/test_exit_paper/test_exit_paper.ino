#include <AccelStepper.h>
#include <MultiStepper.h>

#define MOTOR_DIR 11 
#define MOTOR_PWM 3
#define SPIDER_EXIT_PIN_STP 19
#define SPIDER_EXIT_PIN_DIR 18
#define SPIDER_EXIT_PIN_ENABLE 21
#define SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM 4
#define SPIDER_UPDOWN_PIN_ENDSTOP_UP 12

AccelStepper stepperExit(1, SPIDER_EXIT_PIN_STP, SPIDER_EXIT_PIN_DIR);

void setup() { //Setup runs once//
Serial.begin(9600);
pinMode(MOTOR_DIR, OUTPUT);
pinMode(MOTOR_PWM, OUTPUT);
pinMode(SPIDER_EXIT_PIN_ENABLE, OUTPUT);

}

void loop() { //Loop runs forever//

downSpider();

}

void downSpider(){
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
  unsigned long startMoove = millis();
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  // we start fast.
  if(!bEndStop){
    digitalWrite(SPIDER_UPDOWN_PIN_DIR, LOW);
    for(int i = SPIDER_UPDOWN_LOW_SPEED; i <= SPIDER_UPDOWN_MAX_SPEED; i++){
      currentSpeed = i;
      analogWrite(SPIDER_UPDOWN_PIN_PWM, i); //max speed.
      delay(5);
    }
  }
  // and slow down after 4 seconds.
  unsigned long currentMillis = startMoove;
  while (!bEndStop) { 
    if(currentMillis - startMoove > 4000){
      currentSpeed = SPIDER_UPDOWN_LOW_SPEED;
      analogWrite(SPIDER_UPDOWN_PIN_PWM, currentSpeed); // mid speed
    }
    currentMillis = millis();
    bEndStop = !digitalRead(SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM);
  }
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  currentSpeed = 0;
  digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
}


void runDelivery(){
  //we only go backward for a period of time. The stepper is not mandatory and can be a dc motor. I got a spare stepper on hand...
  digitalWrite(SPIDER_EXIT_PIN_ENABLE, LOW);
  stepperExit.setCurrentPosition(0);
  stepperExit.moveTo(1);
  stepperExit.run();
  deliveryRunning = true;
}
