#ifndef paperDelivery_h
#define paperDelivery_h

#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "constants.h"

void setupDelivery();
void initDelivery();
void runDelivery();
void stopDelivery();

#endif
