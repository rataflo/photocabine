#ifndef rtc_h
#define rtc_h

#include <Wire.h>
#include <RtcDS3231.h>
#include <Adafruit_NeoPixel.h>
#include "constants.h"
#include "tests.h"

void setupRTC();
void checkRTC();
void printDateTime(const RtcDateTime& dt);
void showInitProgress(byte step);
void ceilingWhite();
void ceilingRed();
#endif
