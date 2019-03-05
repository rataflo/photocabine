
#ifndef scissor_h
#define scissor_h

#include <DirectIO.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "constants.h"

void initScissor();
void openScissor();
void closeScissor();
bool readSWScissor();
#endif
