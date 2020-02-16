#ifndef paper_h
#define paper_h

#include <DirectIO.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "constants.h"
#include "shutter.h"
#include "tests.h"


void movePaperFirstShot();
void initPaper();
void movePaperNextShot(byte numShot);
void movePaperOut();

boolean readSWPaper1();
boolean readSWPaper2();
boolean readSWPaper3();
boolean readSWPaper4();

#endif
