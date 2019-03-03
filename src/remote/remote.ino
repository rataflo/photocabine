/*
   SDA A4
   SCL A5
*/
#include <nRF24L01.h>
#include <RF24.h>
#include <DirectIO.h>
#include <LiquidCrystal_I2C.h>
#include "orders.h"
#include "constants.h"


const byte RADIO_ADRESS_EMITTER[6] = "00001";
const byte RADIO_ADRESS_RECEIVER[6] = "00002";
RF24 radio(RADIO_CE, RADIO_CSN); // CE, CSN

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
Input<MENU_BTN1_PIN> btnMenu1;
Input<MENU_BTN2_PIN> btnMenu2;

bool bMainScreen = true;
byte currentMenu = 0;
byte currentLineInMenu = 0;
unsigned long previousMenuMillis = 0;
unsigned long previousMainMenuMillis = 0;
bool bPause = false;
bool bTests = false;
char mode = MODE_PAYING;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(RADIO_ADRESS_EMITTER); // 00001
  radio.openReadingPipe(1, RADIO_ADRESS_RECEIVER); // 00002
  radio.setAutoAck(1);                    // Ensure autoACK is enabled so rec sends ack packet to let you know it got the transmit packet payload
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(250, 2);
  radio.startListening();

  lcd.begin();
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
    if (btnMenu1.read()) {
      nextMenu();
    } else if (!bMainScreen && btnMenu2.read()) {
      doMenu();
    } else if(bMainScreen && currentMillis - previousMainMenuMillis > 60000){ // Refresh main screen periodically
      showMainScreen();
      previousMainMenuMillis = currentMillis;
    }
  }
}

/*
   Show main screen
*/
void showMainScreen() {
  bMainScreen = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   -- NO-MATON --   ");

  // Ask & display infos
  char state = sendOrderAndWaitForChar(ORDER_GET_STATUS);
  lcd.setCursor(0, 1);
  lcd.print("Status:");
  lcd.print(state == RESPONSE_STATUS_RUNNING ? "Running      " : state == RESPONSE_STATUS_TEST ? "Tests        " : "Pause        ");
  bPause = state == RESPONSE_STATUS_PAUSE ? true : false;
  bTests = state == RESPONSE_STATUS_TEST ? true : false;
  delay(100);
  if (!bPause && !bTests) {
    int temp = sendOrderAndWaitForInt(ORDER_TEMP);
    lcd.setCursor(0, 2);
    lcd.print("Temp:");
    lcd.print(temp);
    delay(100);
    mode = sendOrderAndWaitForChar(ORDER_MODE);
    lcd.print(" Mode:");
    lcd.print(mode == MODE_PAYING ? "$$" : mode == MODE_FREE_PRICE ? "$/Free" : "Free");
    delay(100);
    int totStrip = sendOrderAndWaitForInt(ORDER_NBSTRIP);
    lcd.setCursor(0, 3);
    lcd.print("Strip:");
    lcd.print(totStrip);
    delay(100);
    float totMoney = sendOrderAndWaitForFloat(ORDER_TOTCENT);
    lcd.print(" $:");
    lcd.print(totMoney);
  }
}

void printMsgToLCD(char *message, bool isSelected) {
  char *tmp;
  if (isSelected) {
    strcpy(tmp, sel);
  } else {
    strcpy(tmp, espace);
  }
  strcat(tmp, message);
  // add space for 20 caracters
  for (int i = strlen(tmp); i < 20; i++) {
    strcat(tmp, " ");
  }
  lcd.print(tmp);
}

/*
   Display a menu.
*/
void showMenu() {
  int ligneLCD = 0;
  int lineInMenu = 0;

  for (int i = 0; i < TAILLE_MENU; i++) {
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
            printMsgToLCD(mode == MODE_PAYING ? "$$" : mode == MODE_FREE_PRICE ? "$/Free" : "Free",  lineInMenu == currentLineInMenu);
            break;

          default:
            printMsgToLCD(MENUS[i].label, lineInMenu == currentLineInMenu);
        }

        ligneLCD++;
        if (ligneLCD == 4) {
          break;
        }
      }
      lineInMenu++;
    }
  }

  for (int i = ligneLCD; i < 4; i++) {
    lcd.setCursor(0, i);
    lcd.print("                    ");
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
    case 11: // Return menu tests
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
    case 22: // Return menu tests
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
    case 23: // Test switch shutter
      testSwitch(ORDER_SWSHUTTER, MENUS[line].id);
      break;
    case 24: // Test switch scissor
      testSwitch(ORDER_SWSCISSOR, MENUS[line].id);
      break;
    case 25: // Test switch paper 1
      testSwitch(ORDER_SWPAPER1, MENUS[line].id);
      break;
    case 26: // Test switch paper 2
      testSwitch(ORDER_SWPAPER2, MENUS[line].id);
      break;
    case 27: // Test switch paper 3
      testSwitch(ORDER_SWPAPER3, MENUS[line].id);
      break;
    case 28: // Test switch paper 4
      testSwitch(ORDER_SWPAPER4, MENUS[line].id);
      break;
    case 29: // Test switch start btn
      testSwitch(ORDER_SWSTART, MENUS[line].id);
      break;
    case 30: // Test switch spider up
      testSwitch(ORDER_SWUP, MENUS[line].id);
      break;  
    case 31: // Test switch spider bottom
      testSwitch(ORDER_SWDOWN, MENUS[line].id);
      break;  
    case 32: // Test rotate pair
      testSwitch(ORDER_SWROTPAIR, MENUS[line].id);
      break;
    case 33: // Test rotate impair
      testSwitch(ORDER_SWROTIMPAIR, MENUS[line].id);
      break;
    case 34: // Return menu tests
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
    case 38: // Return menu tests
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 39: // Setup Mode
      sendOrder(ORDER_SET_MODE);
      sendOrder(mode == MODE_PAYING ? MODE_FREE_PRICE : mode == MODE_FREE_PRICE ?  MODE_FREE : MODE_PAYING);
      mode = mode == MODE_PAYING ? MODE_FREE_PRICE : mode == MODE_FREE_PRICE ?  MODE_FREE : MODE_PAYING;
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(mode == MODE_PAYING ? "$$" : mode == MODE_FREE_PRICE ? "$/Free" : "Free", true);
      break;
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
    int nbItemMenu = 0;
    for (int i = 0; i < TAILLE_MENU; i++) {
      if (MENUS[i].parentMenu == currentMenu) {
        nbItemMenu++;
      }
    }

    // Case end of menu, return to first line.
    if (currentLineInMenu + 1  >= nbItemMenu) {
      currentLineInMenu =  0;
      showMenu();

    } else if (currentLineInMenu == 3 || currentLineInMenu == 7 || currentLineInMenu == 11) { // Case show next page in menu
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

  sendOrder(order);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(MENUS[idTest - 1].label);

  while (!btnMenu2.read()) {
    if (radio.available()) {
      char state = ' ';
      radio.read(&order, sizeof(order));
      lcd.setCursor(0, 1);
      lcd.print(state);
    }
    delay(MENU_SPEED);
  }
  showMenu();
}

void sendOrder(char order) {
  radio.stopListening();
  radio.write(&order, sizeof(order));
  radio.flush_rx();
  radio.startListening();
}

char sendOrderAndWaitForChar(char order) {
  sendOrder(order);
  while (!radio.available()) {
  }
  char answer = NO_ORDER;
  radio.read(&answer, sizeof(answer));
  return answer;
}

int sendOrderAndWaitForInt(char order) {
  sendOrder(order);
  while (!radio.available()) {
  }
  int answer = 0;
  radio.read(&answer, sizeof(answer));
  return answer;
}

float sendOrderAndWaitForFloat(char order) {
  sendOrder(order);
  while (!radio.available()) {
  }
  float answer = 0;
  radio.read(&answer, sizeof(answer));
  return answer;
}

