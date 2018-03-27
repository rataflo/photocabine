// Shutter
AccelStepper shutter(1, SHUTTER_PIN_STP, SHUTTER_PIN_DIR);
bool bCloseShutter = false; // Shutter is closed?
Input<SHUTTER_ENDSTOP_PIN> endstopShutter;

// LED Matrix
LedControl ledMatrix = LedControl(LED_MATRIX_SDI_PIN, LED_MATRIX_SCL_PIN, LED_MATRIX_CS_PIN, 1); 

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
  
  while(!bCloseShutter){
    boolean bEndStop = false;
    int currentShutterNbStep = shutter.currentPosition();
    if(currentShutterNbStep < -195){ // Digital read at the last time.
      bEndStop = !endstopShutter.read();
    }

    // Flash during rotation.
    if(currentShutterNbStep == -50){
      flashOn();
    } else if(currentShutterNbStep == -150 ){
      flashOff();
    }
    
    if(bEndStop){
      shutter.stop();
      shutter.run();
      shutter.setCurrentPosition(0);
      bCloseShutter = true;
      
    } else {
      shutter.run();
    }
  }
}

/********************************
 *  LED MATRIX FOR COUNTDOWN
 ********************************/
void initLedMatrix(){
  ledMatrix.shutdown(0,false);  // Wake up displays
  ledMatrix.setIntensity(0,1);  // Set intensity levels at the minimum
  ledMatrix.clearDisplay(0);  // Clear Displays
}

void displayNumber(byte numero)
{
  for (int i = 0; i < 8; i++)  
  {
    ledMatrix.setRow(0,i,IMAGES[numero][i]);
  }
}

void showCountdown()
{
  for (int i = 0; i < 6; i++)  
  {
    displayNumber(i);
    delay(1000);
  }
}
