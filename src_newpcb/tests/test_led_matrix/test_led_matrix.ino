#include "LedControl.h"

/**
 * PIN ARDUINO DUEMILANOVE
 * 12 = SDI = DIN
 * 11 = CS
 * 10 = SCL = CLOCK
 */

LedControl lc=LedControl(12,10,11,2);  // Pins: DIN,CLK,CS, # of Display connected

unsigned long delayTime=200;  // Delay between Frames

const byte IMAGES[][8] = {
{
  B00000000,
  B01111110,
  B01100000,
  B01111100,
  B00000110,
  B00000110,
  B01100110,
  B00111100
},{
  B00000000,
  B00001100,
  B00011100,
  B00101100,
  B01001100,
  B01111110,
  B00001100,
  B00001100
},{
  B00000000,
  B00111100,
  B01100110,
  B00000110,
  B00011100,
  B00000110,
  B01100110,
  B00111100
},{
  B00000000,
  B00111100,
  B01100110,
  B00000110,
  B00001100,
  B00110000,
  B01100000,
  B01111110
},{
  B00000000,
  B00011000,
  B00011000,
  B00111000,
  B00011000,
  B00011000,
  B00011000,
  B01111110
},
{
  B00000000,
  B01100110,
  B01100110,
  B00000000,
  B10000001,
  B01000010,
  B00111100,
  B00000000
}};
const int IMAGES_LEN = sizeof(IMAGES)/8;



void setup()
{
  lc.shutdown(0,false);  // Wake up displays

  lc.setIntensity(0,1);  // Set intensity levels

  lc.clearDisplay(0);  // Clear Displays

}


//  Take values in Arrays and Display them
void displayNumber(byte numero)
{
  for (int i = 0; i < 8; i++)  
  {
    lc.setRow(0,i,IMAGES[numero][i]);
  }
}

void loop()
{
// Put #1 frame on both Display
    displayNumber(0);
    delay(1000);
    displayNumber(1);
    delay(1000);
    displayNumber(2);
    delay(1000);
    displayNumber(3);
    delay(1000);
    displayNumber(4);
    delay(1000);
    displayNumber(5);
    delay(1000);
}
