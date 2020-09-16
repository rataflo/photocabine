#include "rtc.h"

RtcDS3231<TwoWire> Rtc(Wire);
Adafruit_NeoPixel ceilingPixels = Adafruit_NeoPixel(CEILING_NBPIXEL, CEILING_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setupRTC(){
  debug("setupRTC", String("begin"));

  ceilingPixels.begin(); 
  for(int i=0;i<CEILING_NBPIXEL;i++){
    ceilingPixels.setPixelColor(i, 255, 0, 0); // Dark
  }
  ceilingPixels.show();
    
  Rtc.Begin();
  
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  
  if (!Rtc.IsDateTimeValid()) 
  {
      if (Rtc.LastError() != 0)
      {
          // we have a communications error
          // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
          // what the number means
          Serial.print("RTC communications error = ");
          Serial.println(Rtc.LastError());
      }
      else
      {
          // Common Causes:
          //    1) first time you ran and the device wasn't running yet
          //    2) the battery on the device is low or even missing
  
          Serial.println("RTC lost confidence in the DateTime!");
  
          // following line sets the RTC to the date & time this sketch was compiled
          // it will also reset the valid flag internally unless the Rtc device is
          // having an issue
  
          Rtc.SetDateTime(compiled);
      }
  }
  
  if (!Rtc.GetIsRunning())
  {
      Serial.println("RTC was not actively running, starting now");
      Rtc.SetIsRunning(true);
  }
  
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) 
  {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      Rtc.SetDateTime(compiled);
  }
  else if (now > compiled) 
  {
      Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled) 
  {
      Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
  
  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
  debug("setupRTC", String("end"));
}

void checkRTC(){
  if (!Rtc.IsDateTimeValid()) 
  {
      if (Rtc.LastError() != 0)
      {
          // we have a communications error
          // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
          // what the number means
          Serial.print("RTC communications error = ");
          Serial.println(Rtc.LastError());
      }
      else
      {
          // Common Causes:
          //    1) the battery on the device is low or even missing and the power line was disconnected
          Serial.println("RTC lost confidence in the DateTime!");
      }
  }

  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  
  //Switch/Turn off lights when time as come.
  /*if(now.Hour() >= HOUR_ON || now.Hour() < HOUR_OFF){
    // Ligth the ceiling
    for(int i=0;i<CEILING_NBPIXEL;i++){
      ceilingPixels.setPixelColor(i, 255,255,255); // white as hell
    }
    debug("lights", String("on"));
  }else if(now.Hour() >= HOUR_OFF || now.Hour() < HOUR_ON){
    // switch off the ceiling
    for(int i=0;i<CEILING_NBPIXEL;i++){
      ceilingPixels.setPixelColor(i, 255, 0, 0); // Dark
    }
    debug("lights", String("off"));
  }
  ceilingPixels.show();*/
}
#define countof(a) (sizeof(a) / sizeof(a[0]))
void printDateTime(const RtcDateTime& dt)
{
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    debug("date/time", String(datestring));
}

void showInitProgress(byte step){

  /*for(int i=0;i<CEILING_NBPIXEL;i++){
      ceilingPixels.setPixelColor(i, 0, 0, 0); // Dark
    }
  for(int i=24;i<24+step;i++){
      ceilingPixels.setPixelColor(i+step, 255, 0, 0); // Dark
    }
    ceilingPixels.show();*/
}

void ceilingWhite(){

  for(int i=0;i<CEILING_NBPIXEL;i++){
    ceilingPixels.setPixelColor(i, 255, 255, 255); // white
  }
  ceilingPixels.show();
}

void ceilingRed(){

  for(int i=0;i<CEILING_NBPIXEL;i++){
    ceilingPixels.setPixelColor(i, 255, 0, 0); // red
  }
  ceilingPixels.show();
}
