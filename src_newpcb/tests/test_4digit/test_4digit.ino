#include "TM1637Display.h"

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   2000

//    A
//   ---
// F | | B
//   -G -
// E | | C
//   ---
//    D

const uint8_t SEG_FULL[] = {
  SEG_A | SEG_F | SEG_G | SEG_E,                   // F
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,           // U
  SEG_F | SEG_E | SEG_D,                           // L
  SEG_F | SEG_E | SEG_D,                           // L
  };

TM1637Display display(CLK, DIO);

void setup()
{
  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setBrightness(0x0f);

  // All segments on
  display.setSegments(data);
}

void loop()
{
  
  /*display.showNumberDecEx(1234,(0x80 >> 0), false, 4, 0);// Show 1.234
  delay(1000);
  display.showNumberDecEx(123,(0x80 >> 0), false, 4, 0);// Show X.123
  delay(1000);
  display.showNumberDecEx(20,(0x80 >> 1), false, 4, 0);// Show XX.20
  delay(1000);
  display.showNumberDecEx(i,(0x80 >> 1), true, 4, 0);*/// Show 00.20
  delay(1000);
  int i = 200;
  while(i >=0){
    setDigits(i);
    i = i-10;
    delay(500);
  }

  /*display.showNumberDecEx(200,(0x80 >> 1), true, 4, 0);// Show 02.00
  delay(1000);*/
  display.setSegments(SEG_FULL);
  delay(1000);
}

/*
 * Display digits on 4 * 7 segments display.
 * We assume number are factor of 10 (min coin = 10cts) like (0.10 or 1.70).
 * and dot sign always at the second position (like 1.20 or 0.10).
 * First digit from left is always off.
 * Last digit is always 0.
 */
void setDigits(int number){
  uint8_t data[] = {0x0, 0x0, 0x0, 0b00111111}; 
  if(number < 100){
    data[1] = display.encodeDigit(0) + 0b10000000;//0.
    data[2] = display.encodeDigit(number / 10);
  } else if(number >= 100 && number < 1000){
    data[1] = display.encodeDigit(number / 100) + 0b10000000;
    data[2] = display.encodeDigit((number / 10) % 10);
  }
  display.setSegments(data, 4, 0);
}

