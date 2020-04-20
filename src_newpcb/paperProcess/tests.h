/*
 * TESTS
 */
#ifndef tests_h
#define tests_h

#include <Arduino.h>
#include "constants.h"
#include "orders.h"
#include "spider.h"
#include "paperDelivery.h"

void testMode(struct storage *params);
void sendAnswer(boolean answer);

void debug(String functionName, String varValue);
void debug(String functionName, boolean varValue);
void debug(String functionName, byte varValue);
void Serial3Clear();
#endif
