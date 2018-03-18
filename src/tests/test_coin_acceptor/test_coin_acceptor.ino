

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  attachInterrupt(digitalPinToInterrupt(3), coinInterrupt, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("ENABLE");
  digitalWrite(5, HIGH);
  delay(5000);
  Serial.println("DISABLE");
  digitalWrite(5, LOW);
  delay(5000);
  
}

// Interrupt
void coinInterrupt(){
  
  Serial.println("Des sous sous");
  
}
