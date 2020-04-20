/*
 * Billbord effects.
 * 
 */

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NB_PIX 93
#define MARQUEE_PIXEL_PIN 1
#define BRIGHT 80

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NB_PIX, MARQUEE_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup () 
{
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  pixels.setBrightness(BRIGHT);
  pixels.begin();
  erase();
}

void loop () 
{
  flicker();
}

void flicker(){
  for(int j = 10; j < 255; j++){
    for(int i =0; i < NB_PIX; i++){
      pixels.setPixelColor(i, pixels.Color(j,0,0));
    }
    pixels.show();
    delay(10);
  }
  delay(1000);
  for(int j = 255; j > 10; j--){
    for(int i =0; i < NB_PIX; i++){
      pixels.setPixelColor(i, pixels.Color(j,0,0));
    }
    pixels.show();
    delay(10);
  }
}

void erase(){
  for(int i =0; i < NB_PIX; i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  pixels.show();
}
