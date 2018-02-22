#define CW 24 //CW is defined as pin #7//

#define CCW 25 //CCW is defined as pin #8//

void setup() { //Setup runs once//

pinMode(CW, OUTPUT); //Set CW as an output//
digitalWrite(CW,HIGH); 
pinMode(CCW, OUTPUT); //Set CCW as an output//
digitalWrite(CCW, HIGH);
}

void loop() { //Loop runs forever//

delay(1000);

digitalWrite(CW,LOW); //Motor runs clockwise//
delay(1000); //for 1 second//
digitalWrite(CW, HIGH); //Motor stops//
digitalWrite(CCW, HIGH);
delay(1000);
digitalWrite(CCW, LOW);//Motor runs counter-clockwise//

delay(1000); //For 1 second//

digitalWrite(CCW, HIGH); //Motor stops//

}
