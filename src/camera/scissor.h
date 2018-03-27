// Scissor.
AccelStepper scissor(1, SCISSOR_PIN_STP, SCISSOR_PIN_DIR);
unsigned long previousScissorMillis = 0;
int currentScissorNbStep = 200; // Number of current step for fully open scissor.
bool bCloseScissor = false; // True if scissor is closed.
bool bOpenScissor = true; // true if scissor wide open
Input<SCISSOR_ENDSTOP_PIN> endstopScissor;

void initScissor() {
  boolean bEndStop = !endstopScissor.read();
  while (!bEndStop) { 
    scissor.moveTo(scissor.currentPosition() + 1); 
    scissor.run();
    bEndStop = !endstopScissor.read();
  }
  scissor.setCurrentPosition(0);
  bCloseScissor = true;
  bOpenScissor = false;
}

void openScissor() {
  if(!bOpenScissor){
    scissor.moveTo(SCISSOR_STEP_OPENED);
    bOpenScissor = false

    while(!bOpenScissor){
      scissor.run();
      bOpenScissor = scissor.currentPosition() == SCISSOR_STEP_OPENED;
    }
    
    bCloseScissor = false;
    bOpenScissor = true;
  }
}

void closeScissor() {
  if(!bCloseScissor){
    scissor.moveTo(0);
    bCloseScissor = false;

    while(!bCloseScissor){
      scissor.run();
      if(scissor.currentPosition() > -5) {
        bCloseScissor = !endstopScissor.read();
      }
    }

    bCloseScissor = true;
    bOpenScissor = false;
  }
}
