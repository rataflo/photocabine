#include <DirectIO.h>
Input<12> microswitch(false); //true activate pullup


void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT);
  // put your setup code here, to run once:
}

void loop() {
  
  Serial.println(digitalRead(4));
}
