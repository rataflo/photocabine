/*
 * TESTS
 */

void testSwitchUpDown(){
  lcd.setCursor(0, currentLineInMenu % 4);
  boolean bEndStop = !digitalRead(SPIDER_UPDOWN_ENDSTOP_PIN);
  // print out the state of the button
  if(bEndStop) {
    lcd.print("closed              ");
  } else{
    lcd.print("open                ");
  }
}
