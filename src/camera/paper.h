// Paper stepper
AccelStepper paper(1, PAPER_PIN_STP, PAPER_PIN_DIR);
// Opto endstops for paper.
Input<PAPER_SWITCH1_PIN> opto1(true);// Activate pullup
Input<PAPER_SWITCH2_PIN> opto2(true);
Input<PAPER_SWITCH3_PIN> opto3(true);
Input<PAPER_SWITCH4_PIN> opto4(true);

void initPaper() {
  boolean bOpto1 = optoendstop.read();
  // case no paper
  if(!bOpto1){
    while (!bOpto1) { 
      paper.moveTo(paper.currentPosition() - 1); 
      paper.run();
      bEndStop = !optoendstop.read();
    }
  }
  else { // paper inside, check in which position
    boolean bOpto2 = optoendstop2.read();

    if(bOpto2){ // Paper not on the right spot to take shot.
      movePaperFirstShot();
    }
  }
  
  paper.setCurrentPosition(0);
}


void movePaperFirstShot() {
  
}

void movePaperNextShot() {
  
}
