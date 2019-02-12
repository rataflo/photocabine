/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 */
#include <Wire.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <EEPROMex.h>
#include <EEPROMVar.h>
#include "constants.h"
#include "spider.h"
#include "paperDelivery.h"
#include "tests.h"

storage params;
char order = NO_ORDER; // 0 = no order.
bool bWait = true; // true if we freeze movement to allow operation like waiting for paper from camera or we have nothing to do.

void setup() {
  Serial.begin(9600);
  
  // load params from eeprom
  EEPROM.readBlock(EEPROM_ADRESS, params);

  initSpider(params.slots, params.bWait);
  initDelivery();
}

void loop() {
  checkOrder();
  process();
}

void checkOrder(){
  if (Serial.available() > 0) {// if new order coming.
    params.order = Serial.read();
  }

  // we process order only when spider at the top
  if(isSpiderUp()){
    switch(params.order){
      case ORDER_NEW_SLOT: // Need a place for paper.
        // Arm ready on slot 0.
        if(params.slots[0] == SLOT_OPEN){
          Serial.print(RESPONSE_OK);
          Serial.flush();
          params.order = NO_ORDER;
          params.bWait = true;
          
        }else if(params.slots[13] == SLOT_CLOSED){ // Arm on exit slot ready to accept paper.
          if(params.slots[13] == SLOT_CLOSED){ // arm not opened.
            openArm();
            params.slots[13] = SLOT_OPEN;
            params.bWait = false;
          }
        } 
        break;
  
      case ORDER_PAPER_READY: // Paper delivered.
        Serial.print(RESPONSE_OK);
        Serial.flush();
        params.slots[0] = SLOT_PAPER;
        params.order = NO_ORDER;
        params.bWait = false;
        break;

      case ORDER_NB_FREE_SLOT:{ // Get number of freee slot.
        byte nbFreeSlot = 0;
        for(byte i = 0; i < 14; i++){
          if(params.slots[i] == SLOT_CLOSED || params.slots[i] == SLOT_OPEN){
            nbFreeSlot++;
          }
        }
        Serial.print(nbFreeSlot);
        Serial.flush();
        params.order = NO_ORDER;
        break;
      }
      case ORDER_PAPER_PROCESS_READY: // If we are here the init is ready
        Serial.print(RESPONSE_OK);
        Serial.flush();
        params.order = NO_ORDER;
        break;
    }
  }
}

void process(){
  if(!params.bWait){
    // Check if a slot contain paper to dip in tank.
    boolean bProcess = false;
    for(byte i = 0; i < 13; i++){
      if(params.slots[i] == SLOT_PAPER){
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
    if(params.slots[12] == SLOT_PAPER){
      downABitSpider();
    }
    
    rotateSpider(params.slots);

    // If paper need delivery
    if(params.slots[13] == SLOT_PAPER){
      
      unsigned long startMillis = millis();
      unsigned long currentMillis = startMillis;
      while(currentMillis - startMillis < DELIVERY_TIME){
        runDelivery();
        initSpiderUp();
      }
      stopDelivery();
      params.slots[13] == SLOT_OPEN;
      // if no order for a place we close the arm.
      if(params.order != ORDER_NEW_SLOT){
        closeArm();
        params.slots[13] == SLOT_CLOSED;
      }
    }

    EEPROM.updateBlock(EEPROM_ADRESS, params);

    // If nothing to do we stop.
    params.bWait = true;
    for(byte i = 0; i < 13; i++){
      if(params.slots[i] == SLOT_PAPER){
        params.bWait = false;
        break;
      }
    }
  }
}
