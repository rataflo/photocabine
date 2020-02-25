#include <AccelStepper.h>
#include <MultiStepper.h>
#include <DirectIO.h>

#define SPIDER_ROTATE_PIN_DIR 26
#define SPIDER_ROTATE_PIN_STP 27


AccelStepper paper(1, SPIDER_ROTATE_PIN_STP, SPIDER_ROTATE_PIN_DIR);

Input<48> opto1(true);// Activate pullup
Input<49> opto2(true);

void setup() {
  Serial.begin(9600);
  // spider rotation
  /*stepper.setMaxSpeed(1000);
  stepper.setAcceleration(400);
  stepper.moveTo(1000);//406 step for 1 tank move*/

  //shutter
  //stepper.setMaxSpeed(500);
  /*stepper.moveTo(200);
  stepper.setMaxSpeed(220);//max speed shutter 200, paper 500
  stepper.setSpeed(220);  */
  initPaper();
  delay(2000);
  movePaperNextShot();
  delay(2000);
  movePaperNextShot();
  delay(2000);
  movePaperNextShot();
  delay(2000);
  movePaperOut();
  delay(2000);
  movePaperFirstShot();
}

void loop() {
  // put your main code here, to run repeatedly:
  
  

}

void initPaper() {
  Serial.println("init paper");
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(500);
  paper.setAcceleration(500);
  

  int homing = 0;
  boolean bOpto1 = opto1.read();
  // case no paper
  if(!bOpto1){
    Serial.println(bOpto1);
    while (!bOpto1) { 
      paper.moveTo(homing); 
      paper.run();
      homing++;
      delay(5);
      bOpto1 = opto1.read();
    }

    paper.setCurrentPosition(0);
  
    // marche arriére
    int delta = -80;
    paper.moveTo(delta); 
    paper.setMaxSpeed(500);
    paper.setAcceleration(500);
    while(paper.currentPosition() != delta){
      paper.run();
    }
  } else{
    movePaperFirstShot();
  }
  
  
  
  Serial.println("fin init paper");
}



void movePaperFirstShot() {
  Serial.println("movePaperFirstShot");
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(500);
  paper.setAcceleration(500);
  
  boolean bOpto1 = opto1.read();
  Serial.println(bOpto1);
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
  int delta = -80+15;
  paper.moveTo(delta); 
  paper.setMaxSpeed(500);
  paper.setAcceleration(500);
  while(paper.currentPosition() != delta){
    paper.run();
  }
}

void movePaperNextShot() {
  Serial.println("movePaperNextShot");
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(500);
  paper.setAcceleration(500);
  
  paper.moveTo(730); 

  while(paper.currentPosition()!= 730){
    paper.run();
  }
  paper.setCurrentPosition(0);
}

void movePaperOut() {
  
  paper.setCurrentPosition(0);
  paper.setMaxSpeed(500);
  paper.setAcceleration(500);
  
  paper.moveTo(3230); 

  while(paper.currentPosition()!= 3230){
    paper.run();
  }
  paper.setCurrentPosition(0);
}

