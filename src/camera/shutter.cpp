#include "shutter.h"
#include "constants.h"

// Flash
Output<FLASH_PIN> flash;

// Shutter
AccelStepper shutter(1, SHUTTER_PIN_STP, SHUTTER_PIN_DIR);
bool bCloseShutter = false; // Shutter is closed?
Input<SHUTTER_ENDSTOP_PIN> endstopShutter(true);
Output<SHUTTER_PIN_ENABLE> enableShutter;

// LED Matrix
LedControl ledMatrix = LedControl(LED_MATRIX_SDI_PIN, LED_MATRIX_SCL_PIN, LED_MATRIX_CS_PIN, 1); 
unsigned long prevousMillisCountdown = 0;
byte countDown = 5;
bool bFlashOn = false;

void flashOn() {
  flash.write(HIGH);
  bFlashOn = true;
}

void flashOff() {
  flash.write(LOW);
  bFlashOn = false;
}

void initShutter() {
  // stepper shutter
  enableShutter.write(LOW);
  shutter.setMaxSpeed(1000);
  shutter.setAcceleration(400);
  shutter.setCurrentPosition(0);
  int homing = 0;
  boolean bEndStop = !endstopShutter.read();

  // do 3step backward to be sure shutter it's not close of switch.
  /*while (!bEndStop && homing > -4) { 
    shutter.moveTo(homing); 
    shutter.run();
    homing--;
    delay(5);
    bEndStop = !endstopShutter.read();
  }
  shutter.setCurrentPosition(0);
  shutter.setMaxSpeed(1000);
  shutter.setAcceleration(400);
  homing = 0;*/
  
  while (!bEndStop) { 
    shutter.moveTo(homing); 
    shutter.run();
    homing++;
    delay(5);
    bEndStop = !endstopShutter.read();
  }
  shutter.setCurrentPosition(0);
  enableShutter.write(HIGH);
  bCloseShutter = true;
}

void takeShot() {
  // TODO calculate duration of the shot.
  enableShutter.write(LOW);
  shutter.setMaxSpeed(1000);
  shutter.setAcceleration(400);
  shutter.moveTo(200);
  bCloseShutter = false;
  
  while(!bCloseShutter){
    boolean bEndStop = false;
    int currentShutterNbStep = shutter.currentPosition();
    if(currentShutterNbStep > 195){ // Digital read at the last time.
      bEndStop = !endstopShutter.read();
    }

    // Flash during rotation.
    if(currentShutterNbStep == 50){
      flashOn();
    } else if(currentShutterNbStep == 150 ){
      flashOff();
    }
    
    if(bEndStop){
      shutter.stop();
      shutter.run();
      shutter.setCurrentPosition(0);
      enableShutter.write(HIGH);
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
  countDown = 5;
  displayNumber(countDown);
  prevousMillisCountdown = millis();
}

void refreshCountdown()
{
  unsigned long currentMillis = millis();
  if(currentMillis - prevousMillisCountdown >= 1000){
    prevousMillisCountdown = currentMillis;
    countDown--;
    displayNumber(countDown);
  }
}

void showArrowDown()
{
  for (int i = 0; i < 8; i++)  
  {
    ledMatrix.setRow(0,i,ARROWDOWN[i]);
  }
}

void clearLedMatrix(){
  ledMatrix.clearDisplay(0);
}

byte getCountDown(){
  return countDown;
}

boolean isFlashOn(){
  return bFlashOn;
}

