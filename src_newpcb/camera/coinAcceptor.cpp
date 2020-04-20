#include "coinAcceptor.h"


volatile byte cents;
const uint8_t SEG_FULL[] = {
  SEG_A | SEG_F | SEG_G | SEG_E,                   // F
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,           // U
  SEG_F | SEG_E | SEG_D,                           // L
  SEG_F | SEG_E | SEG_D,                           // L
  };

const uint8_t SEG_FREE[] = {
  SEG_A | SEG_F | SEG_G | SEG_E,                   // F
  SEG_G | SEG_E,                                   // R
  SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,           // E
  SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,           // E
  };

const uint8_t SEG_BUSY[] = {
  SEG_F | SEG_G | SEG_C | SEG_D | SEG_E,           // B
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,           // U
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // S
  SEG_F | SEG_G | SEG_B | SEG_C,                   // Y
  };

TM1637Display coinSegment(COIN_SEGMENT_CLK_PIN, COIN_SEGMENT_DIO_PIN);

// Coin acceptor
Output<ENABLE_COIN_PIN> enableCoin;

// btn Start + LED
Input<START_BTN_PIN> startBtn(true);
Output<LED_START_BTN_PIN> startLED;
bool bCoinEnabled = false;
bool bStartLedOn = false;
unsigned long oldInterruptMillis;
bool bRefreshSeg = true;

boolean manageCoinsAndStart(byte mode){
  boolean bStart = false;
  refreshCoinSegment(mode);
  
  switch(mode){
    case MODE_PAYING:
      if(cents >= PRICE_CTS){
        showArrowDown();
        startLedOn();
        bStart = !startBtn.read();
      }
      break;
    case MODE_FREE_PRICE:
      if(cents >= FREE_PRICE_CTS){
        showArrowDown();
        startLedOn();
        bStart = !startBtn.read();
      }
      break;
    case MODE_FREE:
      startLedOn();
      bStart = !startBtn.read();
      break;
  }

  if(bStart){
    startLedOff();
    disableCoinAcceptor();
    coinSegment.setSegments(SEG_BUSY);
    ceilingWhite();
    auxOn();
    incrementCounter();
  }

  return bStart;
}

/*
 * Display digits on 4 * 7 segments display.
 * We assume number are factor of 10 (min coin = 10cts) like (0.10 or 1.70).
 * and dot sign always at the second position (like 1.20 or 0.10).
 * First digit from left is always off.
 * Last digit is always 0.
 */
void setCoinDigit(int number){
  uint8_t data[] = {0x0, 0x0, 0x0, 0b00111111}; 
  if(number < 100){
    data[1] = coinSegment.encodeDigit(0) + 0b10000000;//0.
    data[2] = coinSegment.encodeDigit(number / 10);
  } else if(number >= 100 && number < 1000){
    data[1] = coinSegment.encodeDigit(number / 100) + 0b10000000;
    data[2] = coinSegment.encodeDigit((number / 10) % 10);
  }
  coinSegment.setSegments(data);
}


/*
 * Display FULL ont segment display.
 */
void coinSegmentFull(){
  coinSegment.setSegments(SEG_FULL);
}

/*
 * Refresh if needed according to cents the segment.
 */
void refreshCoinSegment(byte mode){
  if(bRefreshSeg){
    if(mode == MODE_PAYING){
      setCoinDigit(PRICE_CTS - cents);
    } else if (mode == MODE_FREE_PRICE){
      setCoinDigit(cents);
    } else {
      coinSegment.setSegments(SEG_FREE);
    }
    bRefreshSeg = false;
  }
}

/*
 * Init coin segment.
 * Brigthness is from 1 to 7.
 */
void initCoinSegment(){
  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  coinSegment.setBrightness(1); // min brightness (max 7)
  coinSegment.setSegments(data);  // All segments off
}

/*
 *  COIN ACCEPTOR
 */
 
// Interrupt main loop each time a pulse from coin acceptor is coming.
// 1 pulse = 5cts. First pulse not counted.
void coinInterrupt(){
  unsigned long currentMillis = millis();
  // Check the duration from previous pulse. Avoid pulse from static electricity.
  unsigned long difference = currentMillis - oldInterruptMillis;
  oldInterruptMillis = currentMillis;
  if(difference < 135 && difference >125){
    cents += bCoinEnabled ? 10 : 0;
  }
  bRefreshSeg = true;
}

void disableCoinAcceptor(){
  detachInterrupt(digitalPinToInterrupt(COIN_PIN));
  enableCoin.write(LOW);
  //setCoinDigit(0);
  bCoinEnabled = false;
  cents = 0;
}

void enableCoinAcceptor(byte mode){
  if(mode != MODE_FREE){
    pinMode(COIN_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(COIN_PIN), coinInterrupt, FALLING);
    enableCoin.write(HIGH);
    bCoinEnabled = true;
    cents = 0;
  }
  bRefreshSeg = true;
  refreshCoinSegment(mode);
}

void startLedOn() {
  debug("startLedOn", "begin");
  startLED.write(HIGH);
  bStartLedOn = true;
}

void startLedOff() {
  debug("startLedOff", "begin");
  startLED.write(LOW);
  bStartLedOn = false;
}

boolean isCoinEnabled(){
  return bCoinEnabled;
}

boolean isStartLedOn(){
  return bStartLedOn;
}

bool readSWStart(){
  return startBtn.read();
}

void waitForStart(){
  while(startBtn.read()){
    // do nothing.
  }
}

void incrementCounter(){
  digitalWrite(COUNT_PIN, HIGH);
  delay(50);
  digitalWrite(COUNT_PIN, LOW);
}
