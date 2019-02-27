#include "tests.h"


void testMode(RF24 radio){
  Serial.println("testMode");
  char order = NO_ORDER;
  byte answer;
  while (order != EXIT_TEST){
    // check for an incoming order.
    radio.startListening();
    delay(5); // leave time to receive message.
    if (radio.available()) {
      radio.read(&order, sizeof(order));
      Serial.println(order);
    }
    radio.stopListening();
    
    switch(order){
      case ENTER_TEST:
        Serial2.print(ENTER_TEST);
        Serial2.flush();
        break;
      case EXIT_TEST:
        Serial2.print(EXIT_TEST);
        Serial2.flush();
        Serial.print(RESPONSE_OK);
        Serial.flush();
        break;
      case ORDER_SWUP: // test upper switch.
        //Serial.println("swup");
        Serial2.print(ORDER_SWUP);
        Serial2.flush();
        answer = ORDER_SWUP;
        radio.write(&answer, sizeof(answer));
        break; 
      case ORDER_SWDOWN: // Test switch bottom
        Serial2.print(ORDER_SWDOWN);
        Serial2.flush();
        answer = ORDER_SWDOWN;
        radio.write(&answer, sizeof(answer));
        break;
      case ORDER_GET_STATUS:
        answer = RESPONSE_STATUS_TEST;
        radio.write(&answer, sizeof(answer));
        break;
    }
    
    // check from answer from paper process.
    if(Serial2.available() > 0){
      answer = Serial2.read();
      radio.write(&answer, sizeof(answer));
    }
  }
  radio.startListening();
}
