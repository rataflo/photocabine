// Paper stepper
AccelStepper paper(1, PAPER_PIN_STP, PAPER_PIN_DIR);
// Opto endstops for paper.
Input<PAPER_SWITCH1_PIN> opto1(true);// Activate pullup
Input<PAPER_SWITCH2_PIN> opto2(true);
Input<PAPER_SWITCH3_PIN> opto3(true);
Input<PAPER_SWITCH4_PIN> opto4(true);

void movePaperFirstShot() {
  paper.setCurrentPosition(0);
  boolean bOpto1 = opto1.read();
  int homing = 0;
  while (bOpto1) { 
    paper.moveTo(homing); 
    paper.run();
    homing--;
    delay(5);
    bOpto1 = opto1.read();
  }
}

void initPaper() {
  Serial.println("init paper");
  paper.setMaxSpeed(1000);
  paper.setAcceleration(400);
  paper.setCurrentPosition(0);

  int homing = 0;
  boolean bOpto1 = opto1.read();
  Serial.println(bOpto1);
  // case no paper
  if(!bOpto1){
    while (!bOpto1) { 
      paper.moveTo(homing); 
      paper.run();
      homing++;
      delay(5);
      bOpto1 = opto1.read();
      Serial.println(bOpto1);
    }
  }
  else { // paper inside, check in which position
    boolean bOpto2 = opto2.read();

    if(bOpto2){ // Paper not on the right spot to take shot.
      movePaperFirstShot();
    }
  }
  
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(1000);
  paper.setAcceleration(400);
  Serial.println("fin init paper");
}




void movePaperNextShot() {
  paper.setCurrentPosition(0);
  paper.moveTo(200); 
  
  while(!paper.currentPosition()== NB_STEP_ONE_SHOT){
    paper.run();
  }
}
