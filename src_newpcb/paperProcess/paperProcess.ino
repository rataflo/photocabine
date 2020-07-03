/**
 * No-Maton paper process code.
 * Flo Gales 2019
 * License rien à branler / Do what the fuck you want.
 * 
 * Libraries used:
 * 
 * Do all the processing of paper, dip in tank and exit.
 * Communicate with camera by Serial3 (cf orders.h)
 */
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "constants.h"
#include "orders.h"
#include "spider.h"
#include "paperDelivery.h"
#include "tests.h"

/*
 * WORK VARIABLES
 */ 
storage params;
char order = NO_ORDER; // 0 = no order.
bool bWait = true; // true if we freeze movement to allow operation like waiting for paper from camera or we have nothing to do.
float tempC = 0;
unsigned long lastCallTemp = 0;

// Temperature probe
OneWire oneWire(TEMP_PIN); 
DallasTemperature tempProbe(&oneWire);
DeviceAddress tempProbAdress;

void setup() {
  #ifdef DEBUG_MODE
    Serial.begin(9600);
  #endif
  Serial3.begin(9600);
  
  // Interrupt for incoming order TODO: test without
  //attachInterrupt(digitalPinToInterrupt(ORDER_INTERRUPT_PIN), emergencyStop, FALLING);
  
  // Temperature
  tempProbe.begin();
  tempProbe.getAddress(tempProbAdress, 0);
  tempProbe.setResolution(tempProbAdress, 9);
  getTemperature();
  
  setupSpider();
  setupDelivery();
  
  // load params from eeprom
  EEPROM.readBlock(EEPROM_ADRESS, params);
  Serial.print("checkCode=");Serial.print("=");Serial.println(params.checkCode);
  // Check verif code, if not correct init eeprom.
  if(params.checkCode != 222){
    params.checkCode = 222;
    params.tankTime = TANK_TIME;
    initSlots(&params);
  }
  debug("checkCode", params.checkCode);
  debug("tankTime", String(params.tankTime));
  for(int i = 0; i <14; i++){
    debug("slot i", params.slots[i]);
  }
  
  // if paper on slot or arm opened, finalize process.
  /*for(int i = 0; i <14; i++){
    if(params.slots[i] == SLOT_PAPER){
      bWait = false;
      upSpider(SPIDER_UPDOWN_LOW_SPEED);
      while(!bWait){
        process();
      }
    }
    if(params.slots[13] == SLOT_OPEN){
      rotateSpider(&params);
      closeArm(&params);
    }
  }*/
  //setupDelivery();
  //runStepper();
  initSlots(&params);
  initSpider(&params);
  //openArm(&params);
  //runDelivery(&params);
}

void loop() {
  // Check bath temp every 10sec.
  unsigned long currentMillis = millis();
  if(currentMillis - lastCallTemp > 10000){
    getTemperature();
    lastCallTemp = currentMillis;
  }
  checkOrder();
  process();
}

void checkOrder(){
  if (Serial3.available()) {
    order = Serial3.read();
    Serial3Clear();
    debug("checkOrder - begin", String(order));
  }
  
  switch(order){
    case ENTER_TEST:
      testMode(&params);
      order = NO_ORDER;
      break;

    case ORDER_TEMP:
      respondToOrder(tempC);
      order = NO_ORDER;
      break;

    case ORDER_NB_FREE_SLOT:{ // Get number of freee slot.
      byte nbFreeSlot = 0;
      for(byte i = 0; i < 14; i++){
        if(params.slots[i] == SLOT_CLOSED || params.slots[i] == SLOT_OPEN){
          nbFreeSlot++;
        }
      }
      respondToOrder(nbFreeSlot);
      order = NO_ORDER;
      break;
    }
    case ORDER_SET_TANK_TIME:
      params.tankTime = Serial3.parseInt();
      EEPROM.updateBlock(EEPROM_ADRESS, params);
      order = NO_ORDER;
      break;
      
    case ORDER_GET_STATUS:
      respondToOrder(RESPONSE_STATUS_RUNNING);
      order = NO_ORDER;
      break;
  }
  
  // Order we process only when spider at the top
  if(isSpiderUp()){ // TODO: no need, do this when spider is up in any case.
    switch(order){
      case ORDER_NEW_SLOT:{ // Need a place for paper.
        // Arm ready on slot 0.
        debug("slots[0]", String(params.slots[0]));
        if(params.slots[0] == SLOT_OPEN){
          digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
          respondToOrder(ORDER_NEW_SLOT_READY);
          bWait = true;
          order = NO_ORDER; 
          
        } else if(params.slots[0] == SLOT_CLOSED){ // Arm not opened
          openArm(&params);
          digitalWrite(SPIDER_ROTATE_PIN_ENABLE, LOW);
          respondToOrder(ORDER_NEW_SLOT_READY);
          bWait = true;
          
        } else if(params.slots[0] == SLOT_NO_ARM){ // no arm
          bWait = false;
        }
        break;
      }
      case ORDER_PAPER_READY:{ // Paper delivered.
        digitalWrite(SPIDER_ROTATE_PIN_ENABLE, HIGH);
        respondToOrder(RESPONSE_OK);
        params.slots[0] = SLOT_PAPER;
        EEPROM.updateBlock(EEPROM_ADRESS, params);
        bWait = false;
        
        // Calculate tank time from temperature + chemical expiration.
        //if(params.tankTime == 0){// 0 = auto tank time.
          if(tempC != 0 && tempC < 50){ //In case temp probe not working
            // Test: increment by 2sec/5°C
            if(tempC <= 20){
              params.tankTime = 60000;
            } else if(tempC > 20 && tempC <= 25){
              params.tankTime = 40000;
            } else if(tempC > 25 && tempC <= 30){
              params.tankTime = 30000;
            } else if(tempC > 30 && tempC <= 35){
              params.tankTime = 28000;
            } else{
              params.tankTime = 26000;
            }
          } else{
            params.tankTime = TANK_TIME;
          }
        //}
        // TODO: tell camera number of slots empty.
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
    debug("process", String("begin"));
    // Check if a slot contain paper to dip in tank.
    boolean bProcess = false;
    for(byte i = 0; i < 14; i++){
      if(params.slots[i] == SLOT_PAPER){
        bProcess = true;
        break;
      }
    }
    debug("bProcess", bProcess);
    //If arm on slot 0 need to be closed.
    if(params.slots[0] == SLOT_OPEN){
      debug("closeArm", String("slot[0]"));
      closeArm(&params);
    } else if(!bProcess && params.slots[13] == SLOT_CLOSED){ // Arm on exit and no other paper to process.
      debug("rotateSpider", String(params.slots[13]));
      rotateSpider(&params);
    }else if(!bProcess && params.slots[13] == SLOT_OPEN){ // Arm open on exit and no other paper to process.
      debug("rotateSpider", String(params.slots[13]));
      downToMiddleSpider();
      blindRotate(&params);
      upSpider(SPIDER_UPDOWN_LOW_SPEED);
      initRotate(&params);
    }

    if(bProcess){
      downSpider();
          
      // Agitate.
      unsigned long startMillis = millis();
      unsigned long currentMillis = startMillis;

      // special case dev tank: TODO un truc plus élégant.
      unsigned long duration = params.tankTime;
      /*if(params.slots[0] == SLOT_PAPER || params.slots[1] == SLOT_PAPER){
          duration = 30000;
      }*/
      /*if(params.slots[10] == SLOT_PAPER){//TODO: Reducer dead
          duration = 120000;
      }*/
      
      while(currentMillis - startMillis < duration){
        agitate();
        currentMillis = millis();
      }
      
      upSpider(SPIDER_UPDOWN_MAX_SPEED);

      delay(2000); //for dripping.
      
      //Last stage, delivery before rotate.
      if(params.slots[12] == SLOT_PAPER){
        debug("runDelivery", "");
        runDelivery(&params);
        
      } else if(params.slots[13] == SLOT_OPEN){
        downToMiddleSpider();
        blindRotate(&params);
        upSpider(SPIDER_UPDOWN_LOW_SPEED);
        
      } else {
        rotateSpider(&params);
        debug("rotateSpider", "");
      }

      // If nothing to do we stop the process.
      bWait = true;
      for(byte i = 0; i < 14; i++){
        if(params.slots[i] == SLOT_PAPER || params.slots[i] == SLOT_OPEN){
          bWait = false;
          break;
        }
      } 
    }
    debug("bWait", bWait);
  }
}

void respondToOrder(char answer){
  Serial3.print(answer);
  Serial3.flush();
  order = NO_ORDER;
}

void respondToOrder(float answer){
  debug("respondToOrder", String(answer, 1));
  Serial3.println(answer, 1);
  Serial3.flush();
  order = NO_ORDER;
}

void respondToOrder(byte answer){
  debug("respondToOrder", answer);
  Serial3.print(answer);
  Serial3.flush();
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
  params.tankTime = tempC;
}
