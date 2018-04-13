#include <DirectIO.h>
Output<11> flash; //true activate pullup

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  flash.write(HIGH);
  delay(100);
  flash.write(LOW);
}
