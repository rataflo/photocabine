#ifndef paper_h
#define paper_h

#include <DirectIO.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "constants.h"
#include "shutter.h"

void movePaperFirstShot();
void initPaper();
void movePaperNextShot(byte numShot);
void movePaperOut();

#endif
