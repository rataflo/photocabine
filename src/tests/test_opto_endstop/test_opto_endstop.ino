#include <DirectIO.h>
Input<11> optoendstop(true); //true activate pullup


void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  
  Serial.println(optoendstop.read());
}
