// X: 22 / 23 
// Y: 24 / 25
// Z: 30 / 31
// Ext: 26 / 27

#define SPIDER_UPDOWN_PIN_DIR 11
#define SPIDER_UPDOWN_PIN_PWM 3 // Use pin 3 because on other pin the pwm is disabled by DirectIO.
#define SPIDER_UPDOWN_PIN_ENDSTOP_UP 12
#define SPIDER_UPDOWN_PIN_ENDSTOP_BOTTOM 4
#define SPIDER_UPDOWN_PWM_SPEED 250

#define SPIDER_ROTATE_PIN_STP 9
#define SPIDER_ROTATE_PIN_DIR 8
#define SPIDER_ROTATE_NBSTEP 406 // Number of steps to mode to one tank
#define SPIDER_ROTATE_ENDSTOP1_PIN 7 // Endstop in front of paper exit.
#define SPIDER_ROTATE_ENDSTOP2_PIN 5 // Endstop 1/14 degrees right from endstop 1.

#define SERVO_ARM 6
#define SERVO_ARM_IDLE_POS 70
#define SERVO_ARM_OPEN_POS 00
#define SERVO_ARM_CLOSE_POS 180

