#include <DirectIO.h>

#define SCISSOR_ENDSTOP_PIN 40
Input<SCISSOR_ENDSTOP_PIN> endstopScissor(true);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
boolean bEndStop = !endstopScissor.read();
Serial.println(bEndStop ? "true": "false");
}
