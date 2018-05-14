


void setup() {
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);
  // put your setup code here, to run once:
}

void loop() {
  
  Serial.println(digitalRead(4));
}
