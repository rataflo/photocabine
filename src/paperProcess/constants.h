// X: 22 / 23 
// Y: 24 / 25
// Z: 30 / 31
// Ext: 26 / 27

// Uncomment to activate test mode on startup, avoid initialisation operation
#define TEST_MODE


#define SPIDER_UPDOWN_PIN_DIR 11
#define SPIDER_UPDOWN_PIN_PWM 3 // Use pin 3 because on other pin the pwm is disabled by DirectIO.
#define SPIDER_UPDOWN_PIN_ENDSTOP_UP 12
#define SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM 4
#define SPIDER_UPDOWN_PWM_SPEED 250

#define SPIDER_ROTATE_PIN_STP 9
#define SPIDER_ROTATE_PIN_DIR 8
#define SPIDER_ROTATE_NBSTEP 406 // Number of steps to mode to one tank
#define SPIDER_ROTATE_ENDSTOP1_PIN A3 // Endstop in front of paper exit. Pair
#define SPIDER_ROTATE_ENDSTOP2_PIN A2 // Endstop 1/14 degrees right from endstop 1. Impair
#define SPIDER_ROTATE_SPEED 500
#define SPIDER_ROTATE_ACCEL 100
#define SPIDER_ROTATE_PIN_ENABLE 5

#define SERVO_ARM 6
#define SERVO_ARM_IDLE_POS 35
#define SERVO_ARM_OPEN_POS_BEGIN 0
#define SERVO_ARM_OPEN_POS_END 150
#define SERVO_ARM_CLOSE_POS_BEGIN 190
#define SERVO_ARM_CLOSE_POS_END 90

#define SPIDER_EXIT_PIN_STP 10
#define SPIDER_EXIT_PIN_DIR 13
#define SPIDER_EXIT_PIN_ENABLE 7
#define SPIDER_EXIT_SPEED 500
#define SPIDER_EXIT_ACCEL 100

#define LEDSTRIP_PIN 2
#define LEDSTRIP_NB 84
#define TANK_TIME 18000 // Default time in each tank (12 sec / 30°C).
#define DELIVERY_TIME 5000 // Time to exit paper

// SLOT STATE
#define SLOT_NO_ARM 0 // 0 = no arm, 
#define SLOT_CLOSED 1 // 1 = arm closed
#define SLOT_OPEN 2 // 2 = arm open, 
#define SLOT_PAPER 3 // 3 = arm open with paper.

// EEPROM data & work variables
#define EEPROM_ADRESS 0
struct storage {
  byte slots[14];
};

