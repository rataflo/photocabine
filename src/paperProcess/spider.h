#ifndef spider_h
#define spider_h


#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include "constants.h"


void initSpider(byte *slots, boolean bFullInit);
void downSpider();
void downABitSpider();
void asyncSpiderUp();
void upSpider();
void rotateSpider(byte *slots);
void agitate();
void initSpiderBottom();
void initSpiderUp();
void initRotate(byte *slots, boolean bFullInit);
boolean isSpiderUp();
void lightStrip(byte *slots);

void initServoArm();
void setServoArmWaitPos();
void openArm();
void closeArm();

#endif
