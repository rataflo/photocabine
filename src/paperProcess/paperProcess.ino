/**
 * No-Maton paper process code.
 * Flo Gales 2019
 * License rien à branler / Do what the fuck you want.
 * 
 * Libraries used:
 * 
 * Do all the processing of paper, dip in tank and exit.
 * Communicate with camera by Serial2 (cf orders.h)
 */
#include <Arduino.h>
#include <EEPROMex.h>
#include <EEPROMVar.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "constants.h"
#include "orders.h"
#include "spider.h"
#include "paperDelivery.h"
#include "tests.h"

/*
 * GLOBAL VARIABLES
 */ 
storage parametres;
byte slots[14]; // State of each arm (closed, open, open with paper);
char order = NO_ORDER; // 0 = no order.
bool bWait = true; // true if we freeze movement to allow operation like waiting for paper from camera or we have nothing to do.
//bool bCloseArm = false; 
float tempC = 0;

// Temperature probe
OneWire oneWire(TEMP_PIN); 
DallasTemperature tempProbe(&oneWire);
DeviceAddress tempProbAdress;

void setup() {
  #ifdef DEBUG_MODE
    Serial.begin(9600);
  #endif
  Serial2.begin(9600);
  
  // Interrupt for incoming order
  attachInterrupt(digitalPinToInterrupt(ORDER_INTERRUPT_PIN), emergencyStop, FALLING);
  
  // Temperature
  tempProbe.begin();
  tempProbe.getAddress(tempProbAdress, 0);
  tempProbe.setResolution(tempProbAdress, 9);

  initSlots();

  // load params from eeprom
  EEPROM.readBlock(EEPROM_ADRESS, parametres);
  memcpy(slots, parametres.slots, sizeof(parametres.slots));

  // Check verif code, if not correct init eeprom.
  if(parametres.checkCode != 222){
    parametres.checkCode = 222;
    parametres.isRunning = false;
    parametres.tankTime = TANK_TIME;
    memcpy(parametres.slots, slots, sizeof(slots));
    EEPROM.writeBlock(EEPROM_ADRESS, parametres);
  }
  
  setupSpider();
  setupDelivery();

  // If previously running we enter test mode to avoid any problem.
  if(parametres.isRunning){
    testMode();
    parametres.isRunning = false;
    EEPROM.updateBlock(EEPROM_ADRESS, parametres);
  }

  initSpider(slots);
}

void loop() {
  getTemperature();
  checkOrder();
  process();
}

void checkOrder(){
  if (Serial2.available()) {
    order = Serial2.read();
    serial2Clear();
    debug("checkOrder - begin", String(order));
  }
  
  switch(order){
    case ENTER_TEST:
      testMode();
      order = NO_ORDER;
      break;

    case ORDER_TEMP:
      respondToOrder(tempC);
      order = NO_ORDER;
      break;

    case ORDER_NB_FREE_SLOT:{ // Get number of freee slot.
      byte nbFreeSlot = 0;
      for(byte i = 0; i < 14; i++){
        if(slots[i] == SLOT_CLOSED || slots[i] == SLOT_OPEN){
          nbFreeSlot++;
        }
      }
      respondToOrder(nbFreeSlot);
      order = NO_ORDER;
      break;
    }
    case ORDER_SET_TANK_TIME:
      parametres.tankTime = Serial2.parseInt();
      EEPROM.updateBlock(EEPROM_ADRESS, parametres);
      order = NO_ORDER;
      break;
      
    case ORDER_GET_STATUS:
      respondToOrder(RESPONSE_STATUS_RUNNING);
      order = NO_ORDER;
      break;
  }
  
  // Order we process only when spider at the top
  if(isSpiderUp()){
    switch(order){
      case ORDER_NEW_SLOT:{ // Need a place for paper.
        // Arm ready on slot 0.
        debug("slots[0]", String(slots[0]));
        if(slots[0] == SLOT_OPEN){
          digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
          respondToOrder(ORDER_NEW_SLOT_READY);
          bWait = true;
          
        } else if(slots[0] == SLOT_CLOSED){ // Arm not opened
          openArm();
          slots[0] = SLOT_OPEN;
          digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
          respondToOrder(ORDER_NEW_SLOT_READY);
          bWait = true;
          
        } else if(slots[0] == SLOT_NO_ARM){ // no arm
          bWait = false;
        }
        order = NO_ORDER; 
        break;
      }
      case ORDER_PAPER_READY:{ // Paper delivered.
        digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
        respondToOrder(RESPONSE_OK);
        slots[0] = SLOT_PAPER;
        bWait = false;
        parametres.isRunning = true;
        EEPROM.updateBlock(EEPROM_ADRESS, parametres);
        // TODO: calculate tank time from temperature.
        order = NO_ORDER;
        break;
      }
      case ORDER_SPIDER_READY:{ // If we are here the init is ready
        respondToOrder(RESPONSE_OK);
        order = NO_ORDER;
        break;
      }
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

    //If arm on slot 0 need to be closed.
    if(slots[0] == SLOT_OPEN){
      closeArm();
    } else if(!bProcess && (slots[13] == SLOT_OPEN || slots[13] == SLOT_CLOSED)){ // Arm on exit and no other paper to process.
      rotateSpider(slots);
    }

    if(bProcess){
      //Calculate tank time.

      downSpider();
          
      // Agitate.
      unsigned long startMillis = millis();
      unsigned long currentMillis = startMillis;
      while(currentMillis - startMillis < parametres.tankTime){
        agitate();
        currentMillis = millis();
      }
      
      upSpider();
    }

    //Last stage, delivery before rotate.
    if(slots[12] == SLOT_PAPER){
      runDelivery(slots);
      slots[13] = SLOT_OPEN;
    } else {
      rotateSpider(slots);
    }

    // If nothing to do we stop.
    bWait = true;
    for(byte i = 0; i < 13; i++){
      if(slots[i] == SLOT_PAPER || slots[i] == SLOT_OPEN){
        bWait = false;
        break;
      }
    }

    if(!bWait){
      parametres.isRunning = false;
      EEPROM.updateBlock(EEPROM_ADRESS, parametres);
    }
    
  }
}

void respondToOrder(char answer){
  /* UNCOMMENT Serial1.print(answer);
  Serial1.flush();*/
  Serial2.print(answer);
  Serial2.flush();
  order = NO_ORDER;
}

void respondToOrder(float answer){
  debug("respondToOrder", String(answer, 1));
  /* UNCOMMENT Serial1.print(answer);
  Serial1.flush();*/
  Serial2.println(answer, 1);
  Serial2.flush();
  order = NO_ORDER;
}

void respondToOrder(byte answer){
  debug("respondToOrder", answer);
  Serial2.print(answer);
  Serial2.flush();
  order = NO_ORDER;
}

/*
 * Interrupt for pause/emergency stop.
 */
void emergencyStop(){
  // halt spider
  analogWrite(SPIDER_UPDOWN_PIN_PWM, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  // Emergency stop, endless loop. It's wrong I know...
  while(digitalRead(ORDER_INTERRUPT_PIN) == LOW){
  }
  digitalWrite(LED_BUILTIN, LOW);
  analogWrite(SPIDER_UPDOWN_PIN_PWM, getSpiderCurrentSpeed());
}

void getTemperature(){
  tempProbe.requestTemperatures(); // Send the command to get temperatures
  tempC = tempProbe.getTempC(tempProbAdress);
  debug("temp", String(tempC));
}

void calcTankTime(){
  parametres.tankTime = tempC;
}

void initSlots(){
  // Init the slots
  slots[0] = SLOT_CLOSED; 
  slots[1] = SLOT_NO_ARM; 
  slots[2] = SLOT_CLOSED; 
  slots[3] = SLOT_NO_ARM; 
  slots[4] = SLOT_CLOSED; 
  slots[5] = SLOT_NO_ARM; 
  slots[6] = SLOT_CLOSED; 
  slots[7] = SLOT_NO_ARM; 
  slots[8] = SLOT_CLOSED; 
  slots[9] = SLOT_NO_ARM; 
  slots[10] = SLOT_CLOSED; 
  slots[11] = SLOT_NO_ARM; 
  slots[12] = SLOT_CLOSED;
  slots[13] = SLOT_NO_ARM;
}
