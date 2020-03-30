
#ifndef coinAcceptor_h
#define coinAcceptor_h

#include <DirectIO.h>
#include "TM1637Display.h"
#include "constants.h"
#include "shutter.h"
#include "tests.h"

boolean manageCoinsAndStart(byte mode);
void setCoinDigit(int number);
void coinSegmentFull();
void refreshCoinSegment(byte mode);
void initCoinSegment();
void coinInterrupt();
void disableCoinAcceptor();
void enableCoinAcceptor(byte mode);
void startLedOn();
void startLedOff(); 
boolean isCoinEnabled();
boolean isStartLedOn();
bool readSWStart();

void waitForStart();
#endif
