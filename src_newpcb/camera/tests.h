#ifndef tests_h
#define tests_h

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "constants.h"
#include "orders.h"
#include "shutter.h"
#include "scissor.h"
#include "paper.h"
#include "coinAcceptor.h"

void testMode(RF24 radio);
void sendAnswer(RF24 radio, boolean answer);
void sendAnswer(RF24 radio, char answer);

void debug(String functionName, String varValue);
void debug(String functionName, float varValue);
void debug(String functionName, char varValue);
void debug(String functionName, int varValue);
void debug(String functionName, boolean varValue);
#endif
