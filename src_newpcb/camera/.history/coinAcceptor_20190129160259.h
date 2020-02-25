
#ifndef coinAcceptor_h
#define coinAcceptor_h

#include <DirectIO.h>
#include "TM1637Display.h"
#include "constants.h"
#include "shutter.h"

boolean manageCoinsAndStart();
void setCoinDigit(int number);
void coinSegmentFull();
void refreshCoinSegment();
void initCoinSegment();
void coinInterrupt();
void disableCoinAcceptor();
void enableCoinAcceptor();
void startLedOn();
void startLedOff(); 
boolean isCoinEnabled();
boolean isStartLedOn();

#endif
