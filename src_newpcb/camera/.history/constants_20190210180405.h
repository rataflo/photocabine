#ifndef constants_h
#define constants_h

#include "Arduino.h"

// X: 22 / 23 
// Y: 24 / 25
// Z: 30 / 31
// Ext: 26 / 27


#define SHUTTER_PIN_STP 23
#define SHUTTER_PIN_DIR 22
#define SHUTTER_ENDSTOP_PIN 53
#define SHUTTER_PIN_ENABLE 31

#define LED_MATRIX_SDI_PIN 12 // SDI = DIN
#define LED_MATRIX_CS_PIN 11 // CS
#define LED_MATRIX_SCL_PIN 10 // SCL = CLOCK

#define SCISSOR_PIN_STP 25
#define SCISSOR_PIN_DIR 24
#define SCISSOR_ENDSTOP_PIN 52
#define SCISSOR_SPEED 200
#define SCISSOR_ACCEL 200
#define SCISSOR_STEP_OPENED 350 // Number of step to fully open the scissor.
#define SCISSOR_PIN_ENABLE 33

#define PAPER_PIN_STP 27
#define PAPER_PIN_DIR 26
#define NB_STEP_PAPER_ONE_SHOT 730 // Number of step to move to another shot.
#define NB_STEP_PAPER_OUT 3330 // Number of step to move out paper. //3230
#define PAPER_SPEED 500
#define PAPER_ACCEL 500
#define DELTA_FIRST_SHOT -80 //Delta in step to do after paper reach opto 1.
#define PAPER_SWITCH1_PIN 48
#define PAPER_SWITCH2_PIN 49
#define PAPER_SWITCH3_PIN 50
#define PAPER_SWITCH4_PIN 51
#define PAPER_PIN_ENABLE 35

#define FLASH_PIN 46

#define START_BTN_PIN 45
#define LED_START_BTN_PIN 47

#define MENU_BTN1_PIN 40
#define MENU_BTN2_PIN 41
#define MENU_SPEED 200 // check button menu each 200ms.

#define COIN_PIN 3 // Add 10k/100k pull up resistor on pin to 5V.
#define ENABLE_COIN_PIN 44 // If High coin acceptor is enable. LOW coin acceptor reject coins.
#define PRICE_CTS 200 // Price of a photo in cents.
#define COIN_SEGMENT_CLK_PIN 6
#define COIN_SEGMENT_DIO_PIN 7

#define WAIT_BETWEEN_SHOT 5000 // Wait between shot in ms.



#endif
