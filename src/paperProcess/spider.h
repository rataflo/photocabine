#ifndef spider_h
#define spider_h


#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include "constants.h"


void initSpider(byte *slots);
void downSpider();
void downABitSpider();
void asyncSpiderUp();
void upSpider();
void rotateSpider(byte *slots);
void agitate();
void initSpiderBottom();
void initSpiderUp();
void initRotate(byte *slots);
boolean isSpiderUp();

void initServoArm();
void setServoArmWaitPos();
void openArm();
void closeArm();

#endif
