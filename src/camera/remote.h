
#ifndef remote_h
#define remote_h

#include <DirectIO.h>
#include <LiquidCrystal_I2C.h>
#include "constants.h"
#include "coinAcceptor.h"
#include "paper.h"
#include "scissor.h"
#include "shutter.h"
#include "tests.h"

void initRemote();
void checkMenu();
void showMainScreen();
void printMsgToLCD(String message, bool isSelected);
void showMenu();
void doMenu();
void nextMenu();

#endif
