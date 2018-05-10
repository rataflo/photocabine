
// Paper stepper
AccelStepper paper(1, PAPER_PIN_STP, PAPER_PIN_DIR);
// Opto endstops for paper.
Input<PAPER_SWITCH1_PIN> opto1(true);// Activate pullup
Input<PAPER_SWITCH2_PIN> opto2(true);
Input<PAPER_SWITCH3_PIN> opto3(true);
Input<PAPER_SWITCH4_PIN> opto4(true);

void movePaperFirstShot() {
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(PAPER_SPEED);
  paper.setAcceleration(PAPER_ACCEL);
  
  boolean bOpto1 = opto1.read();
  int homing = 0;
  while (bOpto1) { 
    paper.moveTo(homing); 
    paper.run();
    homing--;
    delay(5);
    bOpto1 = opto1.read();
  }
  paper.setCurrentPosition(0);
  
  // marche arriére
  int delta = DELTA_FIRST_SHOT + 15;
  paper.moveTo(delta); 
  paper.setMaxSpeed(PAPER_SPEED);
  paper.setAcceleration(PAPER_ACCEL);
  while(paper.currentPosition() != delta){
    paper.run();
  }
}



void initPaper() {
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(PAPER_SPEED);
  paper.setAcceleration(PAPER_ACCEL);
  
  int homing = 0;
  boolean bOpto1 = opto1.read();
  // case no paper
  if(!bOpto1){
    while (!bOpto1) { 
      paper.moveTo(homing); 
      paper.run();
      homing++;
      delay(5);
      bOpto1 = opto1.read();
    }

    paper.setCurrentPosition(0);
  
    // marche arriére
    int delta = DELTA_FIRST_SHOT;
    paper.moveTo(delta); 
    paper.setMaxSpeed(PAPER_SPEED);
    paper.setAcceleration(PAPER_ACCEL);
    while(paper.currentPosition() != delta){
      paper.run();
    }
  } else{
    movePaperFirstShot();
  }
}

void movePaperNextShot(byte numShot) {
  showCountdown();
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(PAPER_SPEED);
  paper.setAcceleration(PAPER_ACCEL);

  int delta = NB_STEP_PAPER_ONE_SHOT;
  paper.moveTo(delta); 
  while(paper.currentPosition() < delta){
    paper.run();
    refreshCountdown();
  }

  boolean bOpto = true;
  if(numShot == 2){
    bOpto = opto2.read();
  } else if(numShot == 3){
    bOpto = opto3.read();
  } else if(numShot == 4){
    bOpto = opto4.read();
  }

  // paper not at next stop, force it.
  if(!bOpto){
    paper.setCurrentPosition(0);
    paper.setMaxSpeed(PAPER_SPEED);
    paper.setAcceleration(PAPER_ACCEL);
  
    int homing = 0;
    while (!bOpto) { 
      paper.moveTo(homing); 
      paper.run();
      homing++;
      delay(5);
      if(numShot == 2){
        bOpto = opto2.read();
      } else if(numShot == 3){
        bOpto = opto3.read();
      } else if(numShot == 4){
        bOpto = opto4.read();
      }
    }
    // move forward
    paper.setCurrentPosition(0);
    
    // marche arriére
    int delta = DELTA_FIRST_SHOT + 15;
    paper.moveTo(delta); 
    paper.setMaxSpeed(PAPER_SPEED);
    paper.setAcceleration(PAPER_ACCEL);
    while(paper.currentPosition() != delta){
      paper.run();
    }
  }
  while(countDown > 0){
    refreshCountdown();
  }
}

void movePaperOut() {
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(200);
  paper.setAcceleration(50);

  int delta = NB_STEP_PAPER_OUT;
  paper.moveTo(delta); 

  while(paper.currentPosition()< delta){
    paper.run();
  }
  
}
