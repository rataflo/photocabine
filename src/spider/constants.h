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
#define SPIDER_ROTATE_ENDSTOP1_PIN A3 // Endstop in front of paper exit.
#define SPIDER_ROTATE_ENDSTOP2_PIN A2 // Endstop 1/14 degrees right from endstop 1.
#define SPIDER_ROTATE_SPEED 500
#define SPIDER_ROTATE_ACCEL 100

#define SERVO_ARM 6
#define SERVO_ARM_IDLE_POS 35
#define SERVO_ARM_OPEN_POS 0
#define SERVO_ARM_CLOSE_POS 145

#define SERVO_EXIT 5
#define SWITCH_EXIT A0

#define SPIDER_EXIT_PIN_STP 10
#define SPIDER_EXIT_PIN_DIR 13

#define TANK_TIME 30000 // Default time in each tank (12 sec / 30°C).

