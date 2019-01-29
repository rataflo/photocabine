/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 */
#include <Wire.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include "constants.h"
#include "spider.h"
#include "paperDelivery.h"
#include "tests.h"

/*
 * GLOBAL VARIABLES
 */
byte slots[14];
char order = NO_ORDER; // 0 = no order.
bool bWait = true; // true if we freeze movement to allow operation like waiting for paper from camera or we have nothing to do.

void setup() {
  Serial.begin(9600);
  
  initSpider(slots);
  initDelivery();
}

void loop() {
  checkOrder();
  process();
}

void checkOrder(){
  if (Serial.available() > 0) {// if new order coming.
    order = Serial.read();
  }

  // we process order only when spider at the top
  if(isSpiderUp()){
    switch(order){
      case ORDER_NEW_SLOT: // Need a place for paper.
        // Arm ready on slot 0.
        if(slots[0] == SLOT_OPEN){
          Serial.print(RESPONSE_OK);
          Serial.flush();
          order = NO_ORDER;
          bWait = true;
          
        }else if(slots[13] == SLOT_CLOSED){ // Arm on exit slot ready to accept paper.
          if(slots[13] == SLOT_CLOSED){ // arm not opened.
            openArm();
            slots[13] = SLOT_OPEN;
            bWait = false;
          }
        } 
        break;
  
      case ORDER_PAPER_READY: // Paper delivered.
        Serial.print(RESPONSE_OK);
        Serial.flush();
        slots[0] = SLOT_PAPER;
        order = NO_ORDER;
        bWait = false;
        break;

      case ORDER_NB_FREE_SLOT: // Get number of freee slot.
        byte nbFreeSlot = 0;
        for(byte i = 0; i < 14; i++){
          if(slots[i] == SLOT_CLOSED || slots[i] == SLOT_OPEN){
            nbFreeSlot++;
          }
        }
        Serial.print(nbFreeSlot);
        Serial.flush();
        order = NO_ORDER;
        break;
    }
  }
}

void process(){
  if(!bWait){
    // Check if a slot contain paper to dip in tank.
    boolean bProcess = false;
    for(byte i = 0; i < 13; i++){
      if(slots[i] == SLOT_PAPER){
        bProcess = true;
        break;
      }
    }

    if(bProcess){
      downSpider();
          
      // Agitate.
      unsigned long startMillis = millis();
      unsigned long currentMillis = startMillis;
      while(currentMillis - startMillis < TANK_TIME){
        agitate();
        currentMillis = millis();
      }
      
      upSpider();
    }

    //In case paper go to last stage (delivery) we go down a little bit before rotating.
    if(slots[12] == SLOT_PAPER){
      downABitSpider();
    }
    
    rotateSpider(slots);

    // If paper need delivery
    if(slots[13] == SLOT_PAPER){
      
      unsigned long startMillis = millis();
      unsigned long currentMillis = startMillis;
      while(currentMillis - startMillis < DELIVERY_TIME){
        runDelivery();
        initSpiderUp();
      }
      stopDelivery();
      slots[13] == SLOT_OPEN;
      // if no order for a place we close the arm.
      if(order != ORDER_NEW_SLOT){
        closeArm();
        slots[13] == SLOT_CLOSED;
      }
    }

    // If nothing to do we stop.
    bWait = true;
    for(byte i = 0; i < 13; i++){
      if(slots[i] == SLOT_PAPER){
        bWait = false;
        break;
      }
    }
  }
}
