// Shutter
AccelStepper shutter(1, SHUTTER_PIN_STP, SHUTTER_PIN_DIR);
bool bCloseShutter = false; // Shutter is closed?
Input<SHUTTER_ENDSTOP_PIN> endstopShutter;
unsigned long previousShotMillis = 0;

void initShutter() {
  boolean bEndStop = !endstopShutter.read();
  while (!bEndStop) { 
    shutter.moveTo(shutter.currentPosition() - 1); 
    shutter.run();
    bEndStop = !endstopShutter.read();
  }
  shutter.setCurrentPosition(0);
  bCloseShutter = true;
}

void takeShot() {
  // TODO calculate duration of the shot.
  shutter.moveTo(-201);
  bCloseShutter = false;
}

void checkShotFinished() {
  boolean bEndStop = false;
  bCloseShutter = false;
  int currentShutterNbStep = shutter.currentPosition();
  if(currentShutterNbStep < -195){ // Digital read at the last time.
    bEndStop = !endstopShutter.read();
  }

  if(bEndStop){
    shutter.stop();
    shutter.run();
    shutter.setCurrentPosition(0);
    bCloseShutter = true;
    stepTakeShot++;
    previousShotMillis = currentMillis;
  }
}
