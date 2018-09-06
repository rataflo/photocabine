
//    A
//   ---
// F | | B
//   -G -
// E | | C
//   ---
//    D

const uint8_t SEG_FULL[] = {
  SEG_A | SEG_F | SEG_G | SEG_E,                   // F
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,           // U
  SEG_F | SEG_E | SEG_D,                           // L
  SEG_F | SEG_E | SEG_D,                           // L
  };

TM1637Display coinSegment(COIN_SEGMENT_CLK_PIN, COIN_SEGMENT_DIO_PIN);

// Coin acceptor
Output<ENABLE_COIN_PIN> enableCoin;
volatile byte cents = 0;
unsigned long oldInterruptMillis;// no need to be volatile, exclusively used by coin interrupt.
bool bCoinEnabled = false;

// btn Start + LED
Input<START_BTN_PIN> startBtn(true);
Output<LED_START_BTN_PIN> startLED;

/*********************************
 * COIN 4 * 7 SEGMENT
 *********************************/
 
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
void refreshCoinSegment(){
  if(bCoinEnabled){
    setCoinDigit(PRICE_CTS - cents);
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
 
// interrupt main loop each time a pulse from coin acceptor is coming.
// 1 pulse = 10cts. First pulse not counted.
void coinInterrupt(){
  currentMillis = millis();
  // Check the duration from previous pulse. Avoi pulse from static electricity.
  unsigned long difference = currentMillis - oldInterruptMillis;
  oldInterruptMillis = currentMillis;
  if(difference < 135 && difference >125){
    cents += bCoinEnabled ? 10 : 0;
  }
}

void disableCoinAcceptor(){
  detachInterrupt(digitalPinToInterrupt(COIN_PIN));
  enableCoin.write(LOW);
  bCoinEnabled = false;
  cents = 0;
}

void enableCoinAcceptor(){
  pinMode(COIN_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(COIN_PIN), coinInterrupt, FALLING);
  setCoinDigit(PRICE_CTS);
  enableCoin.write(HIGH);
  bCoinEnabled = true;
  cents = 0;
}

void startLedOn() {
  startLED.write(HIGH);
  bStartLedOn = true;
}

void startLedOff() {
  startLED.write(LOW);
  bStartLedOn = false;
}


