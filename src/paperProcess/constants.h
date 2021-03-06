#ifndef constants_h
#define constants_h

// uncomment to activate debug and serial output.
#define DEBUG_MODE

#define SPIDER_UPDOWN_PIN_DIR 11
#define SPIDER_UPDOWN_PIN_PWM 3 // Use pin 3 because on other pin the pwm is disabled by DirectIO.
#define SPIDER_UPDOWN_PIN_ENDSTOP_UP 12
#define SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM 4
#define SPIDER_UPDOWN_PIN_ENDSTOP_MIDDLE 45
#define SPIDER_UPDOWN_MAX_SPEED 255
#define SPIDER_UPDOWN_LOW_SPEED 95 //TODO: check other speed for sound level.

#define SPIDER_ROTATE_PIN_STP 23
#define SPIDER_ROTATE_PIN_DIR 22
#define SPIDER_ROTATE_NBSTEP 450 // Number of steps to move to one tank
#define SPIDER_ROTATE_ENDSTOP1_PIN 47 // Endstop in front of paper exit. Pair
#define SPIDER_ROTATE_ENDSTOP2_PIN 46 // Endstop 1/14 degrees right from endstop 1. Impair
#define SPIDER_ROTATE_SPEED 500
#define SPIDER_ROTATE_ACCEL 100
#define SPIDER_ROTATE_PIN_ENABLE 25 // TODO: check if rigth pin.


#define SERVO_ARM 6
#define SERVO_ARM_IDLE_POS 35
#define SERVO_ARM_OPEN_POS_BEGIN 0 
#define SERVO_ARM_OPEN_POS_END 140
#define SERVO_ARM_CLOSE_POS_BEGIN 190
#define SERVO_ARM_CLOSE_POS_END 20

#define SPIDER_EXIT_PIN_STP 10
#define SPIDER_EXIT_PIN_DIR 13
#define SPIDER_EXIT_PIN_ENABLE 7
#define SPIDER_EXIT_SPEED 500
#define SPIDER_EXIT_ACCEL 50
//#define SPIDER_EXIT_PIN_SWITCH not connected for now

#define LEDSTRIP_PIN 9 // TODO : Change pin.
#define LEDSTRIP_NB 84
#define TANK_TIME 18000 // Default time in each tank. Temperature between 32°C and 40°C.
#define DELIVERY_TIME 5000 // Time to exit paper

#define TEMP_PIN 8
#define ORDER_INTERRUPT_PIN 2

// SLOT STATE
#define SLOT_NO_ARM 0 // 0 = no arm, 
#define SLOT_CLOSED 1 // 1 = arm closed
#define SLOT_OPEN 2 // 2 = arm open, 
#define SLOT_PAPER 3 // 3 = arm open with paper.

// EEPROM data
#define EEPROM_ADRESS 0 // Lot of update/read so change adress from time to time to avoid corrupted memory.
struct storage {
  byte checkCode = 222;
  int tankTime = TANK_TIME;
  byte slots[14];
};

#endif
