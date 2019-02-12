#include "tests.h"


void testMode(){
  
  byte order = Serial.available() > 0 ? NO_ORDER : EXIT_TEST;
  while (order != EXIT_TEST){
    
    // check for an incoming order.
    if(Serial.available() > 0){
      order = Serial.read(); // Serial.read send byte value as ascii from serial monitor.
    }
    //Serial.println(order);
    switch(order){
      case ENTER_TEST:
        Serial2.print(ENTER_TEST);
        Serial2.flush();
        break;
      case EXIT_TEST:
        Serial2.print(EXIT_TEST);
        Serial2.flush();
        Serial.print(RESPONSE_OK);
        Serial.flush();
        break;
      case ORDER_SWUP: // test upper switch.
        //Serial.println("swup");
        Serial2.print(ORDER_SWUP);
        Serial2.flush();
        break; 
      case ORDER_SWDOWN: // Test switch bottom
        Serial2.print(ORDER_SWDOWN);
        Serial2.flush();
        break;
    }
    order = NO_ORDER;
    
    // check from answer from paper process.
    if(Serial2.available() > 0){
      byte answer = Serial2.read();
      Serial.print("reponse=");
      Serial.println(answer);
      Serial.flush();
    }
  }
}

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
