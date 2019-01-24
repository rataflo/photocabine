#ifndef tests_h
#define tests_h

#include <DirectIO.h>
#include <LiquidCrystal_I2C.h>
#include "constants.h"

void testSwitchScissor(LiquidCrystal_I2C lcd, byte currentLineInMenu);
void testSwitchShutter(LiquidCrystal_I2C lcd, byte currentLineInMenu);
void testSwitchUpDown(LiquidCrystal_I2C lcd, byte currentLineInMenu);
void testStartButton(LiquidCrystal_I2C lcd, byte currentLineInMenu);

#endif
