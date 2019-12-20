

/*
   SDA A4
   SCL A5
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>
#include <Flash.h>
#include "orders.h"
#include "constants.h"
//#include <MemoryUsage.h>


const byte RADIO_ADRESS_EMITTER[6] = "00001";
const byte RADIO_ADRESS_RECEIVER[6] = "00002";
RF24 radio(RADIO_CE, RADIO_CSN); // CE, CSN

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);

bool bMainScreen = true;
byte currentMenu = 0;
byte currentLineInMenu = 0;
unsigned long previousMenuMillis = 0;
unsigned long previousMainMenuMillis = 0;
bool bPause = false;
bool bTests = false;
char mode = MODE_PAYING;
int price_cts = 200;
int free_price_cts = 100;
int tank_time = 18000;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Init");
  
  pinMode(MENU_BTN1_PIN, INPUT_PULLUP);
  pinMode(MENU_BTN2_PIN, INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(RADIO_ADRESS_EMITTER); // 00001
  radio.openReadingPipe(1, RADIO_ADRESS_RECEIVER); // 00002
  radio.setAutoAck(1);                    // Ensure autoACK is enabled so rec sends ack packet to let you know it got the transmit packet payload
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(250, 2);
  radio.startListening();

  
  showMainScreen();
}

void loop() {
  checkMenu();
}

/*****************
  MENU - GO PRO STYLE
******************/

/* Check and do action on both menu button */
void checkMenu() {
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMenuMillis >= MENU_SPEED) {
    previousMenuMillis = currentMillis;
    if (digitalRead(MENU_BTN1_PIN)) {
      nextMenu();
    } else if (!bMainScreen && digitalRead(MENU_BTN2_PIN)) {
      doMenu();
    } else if(bMainScreen && currentMillis - previousMainMenuMillis > 5000){ // Refresh main screen periodically
      showMainScreen();
      previousMainMenuMillis = currentMillis;
    }
  }
}

/*
   Show main screen
*/
void showMainScreen() {
  Serial.println("showMainScreen");
  bMainScreen = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   -- NO-MATON --   ");

  // Ask & display infos
  char state = sendOrderAndWaitForChar(ORDER_GET_STATUS);
  
  lcd.setCursor(0, 1);
  lcd.print("Status:");
  lcd.print(state == RESPONSE_STATUS_RUNNING ? "Running      " : state == RESPONSE_STATUS_TEST ? "Tests        " : state == RESPONSE_STATUS_PAUSE ? "Pause        " : "?            ");
  bPause = state == RESPONSE_STATUS_PAUSE ? true : false;
  bTests = state == RESPONSE_STATUS_TEST ? true : false;

  if (!bPause && !bTests) {
    int temp = sendOrderAndWaitForInt(ORDER_TEMP);
    lcd.setCursor(0, 2);
    lcd.print("Temp:");
    lcd.print(temp);
    mode = sendOrderAndWaitForChar(ORDER_MODE);
    lcd.print(" Mode:");
    lcd.print(mode == MODE_PAYING ? "$$" : mode == MODE_FREE_PRICE ? "$/Free" : mode == MODE_FREE ? "Free" : "?");
    int totStrip = sendOrderAndWaitForInt(ORDER_NBSTRIP);
    lcd.setCursor(0, 3);
    lcd.print("Strip:");
    lcd.print(totStrip);
    float totMoney = sendOrderAndWaitForFloat(ORDER_TOTCENT);
    lcd.print(" $:");
    lcd.print(totMoney);
    price_cts = sendOrderAndWaitForInt(ORDER_GET_PRICE);
    free_price_cts = sendOrderAndWaitForInt(ORDER_GET_FREE_PRICE);
    tank_time = sendOrderAndWaitForInt(ORDER_GET_TANK_TIME);
  }
}

void printMsgToLCD(char *message, bool isSelected) {
  if (isSelected) {
    lcd.print(sel);
  } else {
    lcd.print(espace);
  }
  lcd.print(message);
  // add space for 20 caracters
  for (byte i = strlen(message) + 1; i < 20; i++) {
    lcd.print(" ");
  }
}

/*
   Display a menu.
*/
void showMenu() {
  lcd.clear();
  byte ligneLCD = 0;
  byte lineInMenu = 0;

  for (byte i = 0; i < TAILLE_MENU; i++) {
    if (MENUS[i].parentMenu == currentMenu) {
      if (lineInMenu >= currentLineInMenu) {
        lcd.setCursor(0, ligneLCD);
        // Special cases
        switch (MENUS[i].id) {
          case 2: //Pause
            printMsgToLCD(bPause ? "Resume" : "Pause", lineInMenu == currentLineInMenu);
            break;

          case 5: //Tests
            printMsgToLCD(bTests ? "Stop tests" : "Start tests", lineInMenu == currentLineInMenu);
            break;

          case 39: //Mode
            printMsgToLCD(mode == MODE_PAYING ? "Mode:$$" : mode == MODE_FREE_PRICE ? "Mode:$/Free" : "Mode:Free",  lineInMenu == currentLineInMenu);
            break;

          case 40: // Price
          case 41:// Free Price
            lcd.print(lineInMenu == currentLineInMenu ? ">" : " ");
            lcd.print(MENUS[i].id == 40 ? "Price:" : "Free price:");
            lcd.print(MENUS[i].id == 40 ? price_cts : free_price_cts);
            break;

          default:
            char label[20];
            strcpy_P(label, (PGM_P)pgm_read_word(&(LABELS[i])));
            printMsgToLCD(label, lineInMenu == currentLineInMenu);
        }

        ligneLCD++;
        if (ligneLCD == 4) {
          break;
        }
      }
      lineInMenu++;
    }
  }
}

/*
   What to do on action button
*/
void doMenu() {
  // Get the menu in action
  byte line = 0;
  for (byte i = 0; i < TAILLE_MENU; i++) {
    if (MENUS[i].parentMenu == currentMenu) {
      if (line == currentLineInMenu) {
        line = i;
        break;
      }
      line++;
    }  
  }
  
  // Do action for menu.
  switch (MENUS[line].id) {
    case 1: // Return
      showMainScreen();
      break;
    case 2: // Pause & resume
      sendOrder(bPause ? ORDER_RESUME : ORDER_PAUSE);
      bPause = !bPause;
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(bPause ? "Resume" : "Pause", true);
      break;
    case 3: // Take shot
      sendOrder(ORDER_TAKE_SHOT);
      break;
    case 4: // Setup
      currentMenu = MENUS[line].id;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 5: // Tests
      sendOrder(bTests ? EXIT_TEST : ENTER_TEST);
      bTests = !bTests;
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(bTests ? "Stop tests" : "Start tests", true);
      if(bTests){
        currentMenu = MENUS[line].id;
        currentLineInMenu = 0;
        showMenu();
      }
      break;
    case 6: // Return 0
      sendOrder(EXIT_TEST);
      bTests = false;
      currentMenu = 0;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 7: // Menu test steppers
    case 8: // Menu test microswitchs
    case 9: // Menu test LED
    case 10: // Simul
      currentMenu = MENUS[line].id;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 11: // Return menu shutter
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 12: // Test shutter
      sendOrder(ORDER_SHUTTER);
      break;
    case 13: // Test scissor
      sendOrder(ORDER_SCISSOR);
      break;
    case 14: // Test paper feeder
      sendOrder(ORDER_PAPER_FEEDER);
      break;
    case 15: // Test spider up
      sendOrder(ORDER_UPSPIDER);
      break;
    case 16: // Test spider down
      sendOrder(ORDER_DOWNSPIDER);
      break;
    case 17: // Test spider rotate
      sendOrder(ORDER_ROTATESPIDER);
      break;
    case 18: // Test paper exit
      sendOrder(ORDER_DELIVERYRUN);
      break;
    case 19: // Servo arm open pos
      sendOrder(ORDER_OPENARM);
      break;
    case 20: // Servo arm close pos
      sendOrder(ORDER_CLOSEARM);
      break;
    case 21: // Servo arm idle pos
      sendOrder(ORDER_IDLEARM);
      break;
    case 22: // Return menu switch
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 23: // Test switch shutter
      currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWSHUTTER, MENUS[line].id);
      break;
    case 24: // Test switch scissor
    currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWSCISSOR, MENUS[line].id);
      break;
    case 25: // Test switch paper 1
    currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWPAPER1, MENUS[line].id);
      break;
    case 26: // Test switch paper 2
    currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWPAPER2, MENUS[line].id);
      break;
    case 27: // Test switch paper 3
    currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWPAPER3, MENUS[line].id);
      break;
    case 28: // Test switch paper 4
    currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWPAPER4, MENUS[line].id);
      break;
    case 29: // Test switch start btn
    currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWSTART, MENUS[line].id);
      break;
    case 30: // Test switch spider up
      testSwitch(ORDER_SWUP, MENUS[line].id);
      break;  
    case 31: // Test switch spider bottom
      currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWDOWN, MENUS[line].id);
      break;  
    case 32: // Test rotate pair
      currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWROTPAIR, MENUS[line].id);
      break;
    case 33: // Test rotate impair
      currentMenu = MENUS[line].parentMenu;
      currentLineInMenu = 0;
      testSwitch(ORDER_SWROTIMPAIR, MENUS[line].id);
      break;
    case 34: // Return menu flash
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 35: // Test flash
      sendOrder(ORDER_FLASH);
      break;
    case 36: // Test start btn light
      sendOrder(ORDER_START_LIGHT);
      break;
    case 37: // Test led strip
      sendOrder(ORDER_LEDSTRIP);
      break;
    case 38: // Return menu setup
      currentMenu = 0;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 39: // Setup Mode
      sendOrder(ORDER_SET_MODE);
      mode = mode == MODE_PAYING ? MODE_FREE_PRICE : mode == MODE_FREE_PRICE ?  MODE_FREE : MODE_PAYING;
      sendOrder(mode);
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(mode == MODE_PAYING ? "Mode:$$" : mode == MODE_FREE_PRICE ? "Mode:$/Free" : "Mode:Free", true);
      break;
    case 40:{ // Setup price
      sendOrder(ORDER_SET_PRICE);
      price_cts = price_cts == 500 ? 50 : price_cts + 50; // Max 5eur
      sendParam(price_cts);
      lcd.setCursor(0, currentLineInMenu % 4);
      lcd.print("                    ");
      lcd.setCursor(0, currentLineInMenu % 4);
      lcd.print(">Price:");
      lcd.print(price_cts);
      break;
    }
    case 41:{ // Setup free price
      sendOrder(ORDER_SET_FREE_PRICE);
      free_price_cts = price_cts == 500 ? 50 : price_cts + 50; // Max 5eur
      sendParam(free_price_cts);
      lcd.setCursor(0, currentLineInMenu % 4);
      lcd.print("                    ");
      lcd.setCursor(0, currentLineInMenu % 4);
      lcd.print(">Free Price:");
      lcd.print(free_price_cts);
      break;
    }
    case 42:{ // Setup tank time
      sendOrder(ORDER_SET_FREE_PRICE);
      free_price_cts = price_cts == 500 ? 50 : price_cts + 50; // Max 5eur
      sendParam(free_price_cts);
      lcd.setCursor(0, currentLineInMenu % 4);
      lcd.print("                    ");
      lcd.setCursor(0, currentLineInMenu % 4);
      lcd.print(">Free Price:");
      lcd.print(free_price_cts);
      break;
    }
    case 43: // Return menu simulation
      currentMenu = 0;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 44:{ // Simulate paper process.
      lcd.clear();
      lcd.setCursor(0, 0);
      char label[20];
      strcpy_P(label, (PGM_P)pgm_read_word(&(LABELS[line])));
      lcd.print(label);
      
      sendOrder(ORDER_NEW_SLOT);
      lcd.setCursor(0, 1);
      lcd.print("Send order new slot:OK");
      // wait for slot ready.
      lcd.setCursor(0, 1);
      lcd.print("Wait for slot ready.  ");
      char answer = NO_ORDER;
      while (answer != ORDER_NEW_SLOT_READY){
        if(radio.available()){
          radio.read(&answer, sizeof(answer));
        }
      }
      lcd.setCursor(0, 1);
      lcd.print("Slot ready          ");
      delay(1000);
      sendOrder(ORDER_PAPER_READY);
      lcd.setCursor(0, 1);
      lcd.print("Send paper ready:");
      answer = NO_ORDER;
      while (answer != RESPONSE_OK){
        if(radio.available()){
          radio.read(&answer, sizeof(answer));
        }
      }
      lcd.print("OK");
      lcd.setCursor(0, 2);
      lcd.print("Press button to exit");
      while (!digitalRead(MENU_BTN2_PIN)) {
      }
      showMenu();
    }
  }
}

/*
   Action on button next
*/
void nextMenu() {
  if (bMainScreen) { // if currently on main screen, show first menu.
    bMainScreen = false;
    currentMenu = 0;
    currentLineInMenu = 0;
    showMenu();

  } else {
    // Get number of items in menu.
    byte nbItemMenu = 0;
    for (byte i = 0; i < TAILLE_MENU; i++) {
      if (MENUS[i].parentMenu == currentMenu) {
        nbItemMenu++;
      }
    }
    // Case end of menu, return to first line.
    if (currentLineInMenu + 1  >= nbItemMenu) {
      currentLineInMenu =  0;
      showMenu();

    } else if (currentLineInMenu == 3 || currentLineInMenu == 7 || currentLineInMenu == 11 || currentLineInMenu == 15) { // Case show next page in menu
      currentLineInMenu++;
      showMenu();

    } else { // Move one step in menu
      lcd.setCursor(0, currentLineInMenu % 4);
      lcd.print(espace);
      currentLineInMenu++;
      lcd.setCursor(0, currentLineInMenu % 4);
      lcd.print(sel);
    }
  }
}

void testSwitch(char order, byte idTest) {
  Serial.println("testSwitch - begin");
  sendOrder(order);

  lcd.clear();
  lcd.setCursor(0, 0);
  char label[20];
  strcpy_P(label, (PGM_P)pgm_read_word(&(LABELS[idTest - 1])));
  lcd.print("Switch:");
  lcd.print(label);
  delay(MENU_SPEED);
  while (!digitalRead(MENU_BTN2_PIN)) {
    if (radio.available()) {
      char state = '?';
      radio.read(&state, sizeof(state));
      lcd.setCursor(0, 1);
      lcd.print(state == ORDER_TRUE ? "True " : "False");
      Serial.print("state=");Serial.println(state);
    }
  }
  showMenu();
  Serial.println("testSwitch - end");
}

void sendOrder(char order) {
  Serial.print("sendOrder - begin:"); Serial.println(order);
  radio.stopListening();
  radio.write(&order, sizeof(order));
  radio.flush_rx();
  radio.startListening();
  Serial.println("sendOrder - end");
}

void sendParam(int param) {
  radio.stopListening();
  radio.write(&param, sizeof(param));
  radio.flush_rx();
  radio.startListening();
}

char sendOrderAndWaitForChar(char order) {
  Serial.println("sendOrderAndWaitForChar");
  sendOrder(order);
  unsigned long startOrder = millis();
  unsigned long currMillis = startOrder;
  
  while (!radio.available() && currMillis - startOrder < TIMEOUT_ORDER) {
    currMillis = millis();
  }
  
  char answer = NO_ORDER;
  if(radio.available()){
    radio.read(&answer, sizeof(answer));
  }
  Serial.print("sendOrderAndWaitForChar - end");Serial.println(answer);
  return answer;
}

int sendOrderAndWaitForInt(char order) {
  sendOrder(order);
  unsigned long startOrder = millis();
  unsigned long currMillis = startOrder;
  while (!radio.available() && currMillis - startOrder < TIMEOUT_ORDER) {
    currMillis = millis();
  }
  int answer = 0;
  if(radio.available()){
    radio.read(&answer, sizeof(answer));
  }
  return answer;
}

float sendOrderAndWaitForFloat(char order) {
  sendOrder(order);
  unsigned long startOrder = millis();
  unsigned long currMillis = startOrder;
  while (!radio.available() && currMillis - startOrder < TIMEOUT_ORDER) {
    currMillis = millis();
  }
  float answer = 0;
  if(radio.available()){
    radio.read(&answer, sizeof(answer));
  }
  return answer;
}
