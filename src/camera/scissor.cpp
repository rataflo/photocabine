#include "scissor.h"

// Scissor.
AccelStepper scissor(1, SCISSOR_PIN_STP, SCISSOR_PIN_DIR);
unsigned long previousScissorMillis = 0;
int currentScissorNbStep = 200; // Number of current step for fully open scissor.
bool bCloseScissor = false; // True if scissor is closed.
bool bOpenScissor = true; // true if scissor wide open
Input<SCISSOR_ENDSTOP_PIN> endstopScissor(true);
Output<SCISSOR_PIN_ENABLE> enableScissor;

void initScissor() {
  int homing = 0;
  
  // stepper scissor
  enableScissor.write(LOW);
  scissor.setCurrentPosition(0);
  scissor.setMaxSpeed(SCISSOR_SPEED);
  scissor.setAcceleration(SCISSOR_ACCEL);
  
  boolean bEndStop = !endstopScissor.read();
  while (!bEndStop) { 
    scissor.moveTo(homing); 
    scissor.run();
    homing--;
    delay(5);
    bEndStop = !endstopScissor.read();
  }
  scissor.setCurrentPosition(0);
  scissor.setMaxSpeed(SCISSOR_SPEED);
  scissor.setAcceleration(SCISSOR_ACCEL);
  enableScissor.write(HIGH);
  bCloseScissor = true;
  bOpenScissor = false;
}

void openScissor() {
  if(!bOpenScissor){
    enableScissor.write(LOW);
    scissor.setMaxSpeed(SCISSOR_SPEED);
    scissor.setAcceleration(SCISSOR_ACCEL);
    int delta = SCISSOR_STEP_OPENED;
    scissor.moveTo(delta);

    while(scissor.currentPosition() < delta){
      scissor.run();
    }
    enableScissor.write(HIGH);
    bCloseScissor = false;
    bOpenScissor = true;
  }
}

void closeScissor() {
  if(!bCloseScissor){
    enableScissor.write(LOW);
    scissor.setMaxSpeed(SCISSOR_SPEED);
    scissor.setAcceleration(SCISSOR_ACCEL);
    scissor.moveTo(0);
    while(!bCloseScissor){
      scissor.run();
      if(scissor.currentPosition() < 10) {
        bCloseScissor = !endstopScissor.read();
      }
    }
    enableScissor.write(HIGH);
    bCloseScissor = true;
    bOpenScissor = false;
  }
  scissor.setCurrentPosition(0);
}
