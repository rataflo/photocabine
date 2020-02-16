#ifndef spider_h
#define spider_h

#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include "constants.h"

void setupSpider();
void initSpider(byte *slots);
void downSpider();
void downABitSpider();
void asyncSpiderUp();
void upSpider();
void downToMiddleSpider();
void rotateSpider(byte *slots);
void blindRotate(byte *slots);
void agitate();
void initSpiderBottom();
void initSpiderUp();
void initRotate(byte *slots);
boolean isSpiderUp();
void lightStrip(byte *slots);
byte getSpiderCurrentSpeed();

void initServoArm();
void setServoArmWaitPos();
void openArm();
void closeArm();
void lightFullStrip();
boolean bSpiderImpair();

#endif
