#include <DirectIO.h>
Input<12> microswitch(true); //true activate pullup


void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  // put your setup code here, to run once:
}

void loop() {
  
  Serial.println(digitalRead(A0));
}
