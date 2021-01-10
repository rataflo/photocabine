#include "shutter.h"
#include "constants.h"

// Flash
Output<FLASH_PIN> flash;

//Aux light
Output<AUX_PIN> aux;

// Shutter
AccelStepper shutter(1, SHUTTER_PIN_STP, SHUTTER_PIN_DIR);
bool bCloseShutter = false; // Shutter is closed?
Input<SHUTTER_ENDSTOP_PIN> endstopShutter(true);
Output<SHUTTER_PIN_ENABLE> enableShutter;
Output<SHUTTER_M0> m0Shutter;

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

void auxOn() {
  aux.write(HIGH);
}

void auxOff() {
  aux.write(LOW);
}

void initShutter() {
  initLedMatrix();
  
  // stepper shutter
  m0Shutter.write(LOW);
  enableShutter.write(LOW);
  shutter.setMaxSpeed(SHUTTER_SPEED);
  shutter.setAcceleration(SHUTTER_ACCEL);
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

void testShutter() {
  debug("takeShot", String("begin"));
  // TODO calculate duration of the shot.
  enableShutter.write(LOW);
  shutter.setMaxSpeed(SHUTTER_SPEED);
  shutter.setAcceleration(SHUTTER_ACCEL);
  shutter.moveTo(100);
  
  while(shutter.currentPosition() != 100){
    
      shutter.run();
  }
  enableShutter.write(HIGH);
  debug("takeShot", String("end"));
}

void takeShot(byte mode) {
  debug("takeShot", String("begin"));
  // TODO calculate duration of the shot.
  enableShutter.write(LOW);
  shutter.setMaxSpeed(SHUTTER_SPEED);
  shutter.setAcceleration(SHUTTER_ACCEL);
  shutter.moveTo(100);
  bCloseShutter = false;

  unsigned long currentMillis = 0;
  unsigned long startFlash = 0;
  unsigned long flashTime = mode == 1 ? 100 : 200;
  while(!bCloseShutter){
    currentMillis = millis();
    boolean bEndStop = false;
    int currentShutterNbStep = shutter.currentPosition();
    if(currentShutterNbStep > 90){ // Digital read at the last time.
      bEndStop = !endstopShutter.read();
    }
    
    // Time to flash.
    if(currentShutterNbStep == 50){
      startFlash = currentMillis;
      flashOn();
    } else if(currentShutterNbStep >= 50 && currentMillis - startFlash > flashTime){
      flashOff();
    }
    
    if(bEndStop){
      shutter.stop();
      shutter.setCurrentPosition(0);
      shutter.run();
      enableShutter.write(HIGH);
      bCloseShutter = true;
      
    } else {
      if(currentShutterNbStep >= 100){// Cas shutter get stuck.
        shutter.moveTo(currentShutterNbStep + 1);
      }
      shutter.run();
    }
  }
  clearLedMatrix();
  debug("takeShot", String("end"));
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
  debug("showCountdown", String("begin"));
  countDown = 5;
  displayNumber(countDown);
  prevousMillisCountdown = millis();
}

void refreshCountdown()
{
  unsigned long currentMillis = millis();
  if(currentMillis - prevousMillisCountdown >= 1000){
    prevousMillisCountdown = currentMillis;
    countDown = countDown > 0 ? countDown-1 : 0;
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

boolean readSWShutter(){
  return endstopShutter.read();
}
