
#ifndef coinAcceptor_h
#define coinAcceptor_h

#include <DirectIO.h>
#include "TM1637Display.h"
#include "constants.h"
#include "shutter.h"

boolean manageCoinsAndStart(storage parametres);
void setCoinDigit(int number);
void coinSegmentFull();
void refreshCoinSegment(storage parametres);
void initCoinSegment();
void coinInterrupt();
void disableCoinAcceptor();
void enableCoinAcceptor(storage parametres);
void startLedOn();
void startLedOff(); 
boolean isCoinEnabled();
boolean isStartLedOn();

#endif
