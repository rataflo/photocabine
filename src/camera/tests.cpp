#include "tests.h"

/*
 * TESTS
 */
void testSwitchScissor(LiquidCrystal_I2C lcd, byte currentLineInMenu){
  lcd.setCursor(0, currentLineInMenu % 4);
  boolean bEndStop = !digitalRead(SCISSOR_ENDSTOP_PIN);
  // print out the state of the button
  if(bEndStop) {
    lcd.print("closed              ");
  } else{
    lcd.print("open                ");
  }
}

void testSwitchShutter(LiquidCrystal_I2C lcd, byte currentLineInMenu){
  lcd.setCursor(0, currentLineInMenu % 4);
  boolean bEndStop = !digitalRead(SHUTTER_ENDSTOP_PIN);
  // print out the state of the button
  if(bEndStop) {
    lcd.print("closed              ");
  } else{
    lcd.print("open                ");
  }
}

void testSwitchUpDown(LiquidCrystal_I2C lcd, byte currentLineInMenu){
  //lcd.setCursor(0, currentLineInMenu % 4);
  /*boolean bEndStop = !digitalRead(SPIDER_UPDOWN_ENDSTOP_PIN);
  // print out the state of the button
  if(bEndStop) {
    lcd.print("closed              ");
  } else{
    lcd.print("open                ");
  }*/
}

void testStartButton(LiquidCrystal_I2C lcd, byte currentLineInMenu){
  lcd.setCursor(0, currentLineInMenu % 4);
  boolean bEndStop = !digitalRead(START_BTN_PIN);
  // print out the state of the button
  if(bEndStop) {
    lcd.print("closed              ");
  } else{
    lcd.print("open                ");
  }
}
