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

#endif
