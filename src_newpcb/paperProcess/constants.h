#ifndef constants_h
#define constants_h

// uncomment to activate debug and serial output.
#define DEBUG_MODE

#define SPIDER_UPDOWN_PIN_DIR 5
#define SPIDER_UPDOWN_PIN_PWM 4
#define SPIDER_UPDOWN_PIN_ENDSTOP_UP 34
#define SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM 36 
//#define SPIDER_UPDOWN_PIN_ENDSTOP_MIDDLE 36 TODO: not necessary.
#define SPIDER_UPDOWN_MAX_SPEED 255
#define SPIDER_UPDOWN_LOW_SPEED 70
#define SPIDER_UPDOWN_AGITATE_SPEED 100

#define SPIDER_ROTATE_PIN_STP 9
#define SPIDER_ROTATE_PIN_DIR 8
#define SPIDER_ROTATE_NBSTEP 320 // Number of steps to move to one tank (helpful to position arm on exit slot)
#define SPIDER_ROTATE_CENTER_STEP 30 // Number of steps to do after opto switch reached to put the arm perfectly centered (helpful for paper receiver).
#define SPIDER_ROTATE_ENDSTOP1_PIN 32 // Endstop in front of paper exit. Pair
#define SPIDER_ROTATE_ENDSTOP2_PIN 33 // Endstop 1/14 degrees right from endstop 1. Impair
#define SPIDER_ROTATE_SPEED 600
#define SPIDER_ROTATE_ACCEL 100
#define SPIDER_ROTATE_PIN_ENABLE 7
#define SPIDER_ROTATE_PIN_M0 6

#define SERVO_ARM 45
#define SERVO_ARM_IDLE_POS 48
#define SERVO_ARM_OPEN_POS_BEGIN 0 
#define SERVO_ARM_OPEN_POS_END 150
#define SERVO_ARM_CLOSE_POS_BEGIN 190
#define SERVO_ARM_CLOSE_POS_END 45

#define SPIDER_EXIT_PIN_STP 13
#define SPIDER_EXIT_PIN_DIR 12
#define SPIDER_EXIT_PIN_ENABLE 11
#define SPIDER_EXIT_SPEED 500
#define SPIDER_EXIT_ACCEL 50
#define SPIDER_EXIT_PIN_M0 10

#define LIGHT_PROCESS_PIN 44
#define LIGHT_PROCESS_NB 84

#define TANK_TIME 20000 // Default time in each tank. Temperature between 32°C and 40°C.
#define DELIVERY_TIME 5000 // Time to exit paper

#define TEMP_PIN 37
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
  unsigned int tankTime = TANK_TIME;
  byte slots[14];
};

#endif
