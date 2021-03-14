#ifndef constants_h
#define constants_h

#include "Arduino.h"

// uncomment to activate debug and serial output.
#define DEBUG_MODE

//PINS
#define SHUTTER_PIN_STP 37
#define SHUTTER_PIN_DIR 38
#define SHUTTER_ENDSTOP_PIN 26
#define SHUTTER_PIN_ENABLE 39
#define SHUTTER_M0 36

#define LED_MATRIX_SDI_PIN 12 // SDI = DIN
#define LED_MATRIX_CS_PIN 11 // CS
#define LED_MATRIX_SCL_PIN 10 // SCL = CLOCK

#define SCISSOR_PIN_STP 40
#define SCISSOR_PIN_DIR 41
#define SCISSOR_ENDSTOP_PIN 27

#define SCISSOR_PIN_ENABLE 42
#define SCISSOR_M0 43

#define PAPER_PIN_STP 33
#define PAPER_PIN_DIR 34

#define PAPER_SWITCH1_PIN 30
#define PAPER_SWITCH2_PIN 31
#define PAPER_SWITCH3_PIN 28
#define PAPER_SWITCH4_PIN 29
#define PAPER_PIN_ENABLE 32
#define PAPER_M0 35

#define FLASH_PIN 45
#define AUX_PIN 46
#define START_BTN_PIN 19
#define LED_START_BTN_PIN 18

#define COIN_PIN 3 // Add 10k/100k pull up resistor on pin to 5V.
#define ENABLE_COIN_PIN 4 // If High coin acceptor is enable. LOW coin acceptor reject coins.
#define COIN_SEGMENT_CLK_PIN 6
#define COIN_SEGMENT_DIO_PIN 5

#define COUNT_PIN 8 //Mechanical counter

#define RADIO_CSN 49
#define RADIO_CE 48
#define RADIO_SCK 52 // Fixed SPI pin on arduino mega
#define RADIO_MOSI 51 // Fixed SPI pin on arduino mega
#define RADIO_MISO 50 // Fixed SPI pin on arduino mega
#define RADIO_INTERRUPT 2

#define CEILING_PIXEL_PIN 13
#define CEILING_NBPIXEL 40

#define ORDER_INTERRUPT_PIN 23

// CONSTANTS

const byte RADIO_ADRESS_EMITTER[6] = "00002";
const byte RADIO_ADRESS_RECEIVER[6] = "00001";

#define SHUTTER_SPEED 1000
#define SHUTTER_ACCEL 5500 // 1450 => 1s, 5500 => 1/2s, 22000 => 1/4s, 30000 => max, 191ms less than 1/8s
#define SHUTTER_STEP_REVOL 200 // Number of step for a full rotation of the shutter

#define SCISSOR_SPEED 200
#define SCISSOR_ACCEL 200
#define SCISSOR_STEP_OPENED 350 // Number of step to fully open the scissor.

#define NB_STEP_PAPER_ONE_SHOT 310 // Number of step to move to another shot.
#define NB_STEP_PAPER_OUT 1285 // Number of step to move out paper.
#define PAPER_SPEED 200
#define PAPER_ACCEL 50
#define DELTA_FIRST_SHOT -25 //Delta in step to do after paper reach opto 1. Previous: -80

#define WAIT_BETWEEN_SHOT 5000 // Wait between shot in ms.

// Modes and prices
#define PRICE_CTS 200 // Price of a photo in cents.
#define FREE_PRICE_CTS 100 // Minimum in free price mode
#define MODE_PAYING 0
#define MODE_FREE_PRICE 1
#define MODE_FREE 2

// Define time to switch on & off billboard + ceiling light.
#define HOUR_ON 21
#define MINUTES_ON 45
#define HOUR_OFF 6
#define MINUTES_OFF 0

// EEPROM data & work variables
#define EEPROM_ADRESS 0
struct storage {
  byte checkCode = 0;
  int totStrip = 0;
  byte mode = 2;// O = paying, 1 = Free price, 2 = Free
  float totMoney = 0;
  int price_cts = 200;
  int free_price_cts = 100;
  int tank_time = 18000;
};

#endif
