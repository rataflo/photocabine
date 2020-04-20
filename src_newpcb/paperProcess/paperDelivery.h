#ifndef paperDelivery_h
#define paperDelivery_h

#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "constants.h"
#include "spider.h"

void setupDelivery();
void runDelivery(struct storage *params);
bool isDeliveryRunning();
void runStepper();//TODO: remove after tests
#endif
