#ifndef spider_h
#define spider_h

#include <Arduino.h>
#include <EEPROMex.h>
#include <EEPROMVar.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>
#include "constants.h"
#include "tests.h"

void setupSpider();
void initSpider(struct storage *params);
void downSpider();
void downABitSpider();
void asyncSpiderUp();
void upSpider(byte speedPwm);
void downToMiddleSpider();
void rotateSpider(struct storage *params);
void blindRotate(struct storage *params);
void switchSlots(struct storage *params);
void agitate();
void initSpiderBottom();
void initSpiderUp();
void initRotate(struct storage *params);
boolean isSpiderUp();
void lightStrip(struct storage *params);
byte getSpiderCurrentSpeed();

void initServoArm();
void setServoArmWaitPos();
void openArm(struct storage *params);
void closeArm(struct storage *params);
void lightFullStrip();
boolean bSpiderImpair();
void initSlots(struct storage *params);
void checkCameraPosition();
#endif
