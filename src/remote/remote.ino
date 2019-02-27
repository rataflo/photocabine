/*
   SDA A4
   SCL A5
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <DirectIO.h>
#include <LiquidCrystal_I2C.h>
#include "orders.h"

#define MODE_PAYING 0
#define MODE_FREE_PRICE 1
#define MODE_FREE 2

#define MENU_BTN1_PIN 5
#define MENU_BTN2_PIN 6
#define MENU_SPEED 200 // check button menu each 200ms.

const byte RADIO_ADRESS_EMITTER[6] = "00001";
const byte RADIO_ADRESS_RECEIVER[6] = "00002";

#define RADIO_CSN 8
#define RADIO_CE 7
#define RADIO_SCK 13
#define RADIO_MOSI 11
#define RADIO_MISO 12

RF24 radio(RADIO_CE, RADIO_CSN); // CE, CSN

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
Input<MENU_BTN1_PIN> btnMenu1;
Input<MENU_BTN2_PIN> btnMenu2;
bool bMainScreen = true;
byte currentMenu = 0;
byte currentLineInMenu = 0;
unsigned long previousMenuMillis = 0;

// Define menu structure.
typedef struct
{
  byte id;
  String label;
  byte parentMenu;
} menuItem;

#define TAILLE_MENU 32
const String MENU_RETOUR = "...";

const menuItem MENUS[TAILLE_MENU] = {
  {1, MENU_RETOUR, 0},
  {2, "Pause", 0},
  {3, "Take shot", 0},
  {4, "Setup", 0},
  {5, "Start tests", 0},
  {6, MENU_RETOUR, 5},
  {7, "Motors", 5},
  {8, "Microswitchs", 5},
  {9, "Flash", 5},
  {10, "Relay", 5},
  {11, MENU_RETOUR, 7},
  {12, "Shutter", 7},
  {13, "Scissor", 7},
  {14, "Paper feeder", 7},
  {15, "Spider up/down", 7},
  {16, "Spider rotate", 7},
  {17, "Paper exit", 7},
  {18, MENU_RETOUR, 8},
  {19, "Shutter", 8},
  {20, "Scissor", 8},
  {21, "Paper 1", 8},
  {22, "Paper 2", 8},
  {23, "Paper 3", 8},
  {24, "Paper 4", 8},
  {25, "Start btn", 8},
  {26, "Spider up", 8},
  {27, "Spider down", 8},
  {28, "Rotate 1", 8},
  {29, "Rotate 2", 8},
  {30, MENU_RETOUR, 9},
  {31, "Flash on", 9},
  {32, "Start LED on", 9},
};

String sel = ">";
String espace = " ";

bool bPause = false;
bool bTests = false;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openWritingPipe(RADIO_ADRESS_EMITTER); // 00001
  radio.openReadingPipe(1, RADIO_ADRESS_RECEIVER); // 00002
  radio.setAutoAck(1);                    // Ensure autoACK is enabled so rec sends ack packet to let you know it got the transmit packet payload
  radio.enableAckPayload();
  radio.setPALevel(RF24_PA_MIN);;
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
  Serial.println(state);
  lcd.setCursor(0, 1);
  lcd.print("Status:");
  lcd.print(state == RESPONSE_STATUS_RUNNING ? "Running      " : state == RESPONSE_STATUS_TEST ? "Tests        " : "Pause        ");
  bPause = state == RESPONSE_STATUS_PAUSE ? true : false;
  bTests = state == RESPONSE_STATUS_TEST ? true : false;

  if (!bPause && !bTests) {
    int temp = sendOrderAndWaitForInt(ORDER_TEMP);
    Serial.println(temp);
    lcd.setCursor(0, 2);
    lcd.print("Temp:");
    lcd.print(temp);

    char mode = sendOrderAndWaitForChar(ORDER_MODE);
    lcd.print(" Mode:");
    lcd.print(mode == MODE_PAYING ? "$$" : mode == MODE_FREE_PRICE ? "$/Free" : "Free");

    int totStrip = sendOrderAndWaitForInt(ORDER_NBSTRIP);
    Serial.println(totStrip);
    lcd.setCursor(0, 3);
    lcd.print("Strip:");
    lcd.print(totStrip);
    float totMoney = sendOrderAndWaitForFloat(ORDER_TOTCENT);
    Serial.println(totMoney);
    lcd.print(" $:");
    lcd.print(totMoney);
  }
  Serial.println("fin main screen");

}

void printMsgToLCD(String message, bool isSelected) {
  String tmp;
  if (isSelected) {
    tmp = sel;
  } else {
    tmp = espace;
  }
  tmp += message;
  // add space for 20 caracters
  for (int i = tmp.length(); i < 20; i++) {
    tmp += " ";
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
    menuItem menu = MENUS[i];
    if (menu.parentMenu == currentMenu) {
      if (lineInMenu >= currentLineInMenu) {
        lcd.setCursor(0, ligneLCD);
        // Special cases
        switch (menu.id) {
          case 2: //Pause
            printMsgToLCD(bPause ? "Resume" : "Pause", lineInMenu == currentLineInMenu);
            break;

          case 5: //Tests
            printMsgToLCD(bTests ? "Stop tests" : "Start tests", lineInMenu == currentLineInMenu);
            break;

          default:
            printMsgToLCD(menu.label, lineInMenu == currentLineInMenu);
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
  int line = 0;
  menuItem menu;
  for (int i = 0; i < TAILLE_MENU; i++) {
    menu = MENUS[i];
    if (menu.parentMenu == currentMenu) {
      if (line == currentLineInMenu) {
        break;
      }
      line++;
    }
  }

  // Do action for menu.
  switch (menu.id) {
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
    case 4: // Setup
    case 5: // Tests
      sendOrder(bTests ? EXIT_TEST : ENTER_TEST);
      bTests = !bTests;
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(bTests ? "Stop tests" : "Start tests", true);
      if(bTests){
        currentMenu = menu.id;
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
      currentMenu = menu.id;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 8: // Menu test microswitchs
      currentMenu = menu.id;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 9: // Menu test LED
      currentMenu = menu.id;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 10: // Test relay
      break;
    case 11: // Return menu tests
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 12: // Test shutter
      //takeShot();
      break;
    case 13: // Test
      // close scissor.
      //closeScissor();
      // Open scissor
      ////openScissor();
      // close again
      //closeScissor(); // close scissor.
      break;
    case 18: // Return menu tests
    case 23: // Return menu tests
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
      break;

    case 19: // Test switch shutter.
      testSwitch(ORDER_SWUP, menu.id);

      break;

    case 20: // Test switch scissor.
      delay(200);
      while (!btnMenu2.read()) {
        //testSwitchScissor(lcd, currentLineInMenu);
        delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;

    case 21: // Test switch up
      delay(200);
      while (!btnMenu2.read()) {
        //testSwitchUpDown(lcd, currentLineInMenu);
        delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;

    case 22: // Start button
      delay(200);
      while (!btnMenu2.read()) {
        //testStartButton(lcd, currentLineInMenu);
        delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;

    case 24: // Test flash
      /*if(!isFlashOn()){
        flashOn();
        lcd.setCursor(0, currentLineInMenu % 4);
        lcd.print(">Flash off          ");
        }else{
        flashOff();
        lcd.setCursor(0, currentLineInMenu % 4);
        printMsgToLCD(menu.label, true);
        }*/
      break;

    case 25: // Test led start button
      /*if(!isStartLedOn()){
        startLedOn();
        lcd.setCursor(0, currentLineInMenu % 4);
        lcd.print(">Start LED off      ");
        }else{
        startLedOff();
        lcd.setCursor(0, currentLineInMenu % 4);
        printMsgToLCD(menu.label, true);
        }*/
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
      menuItem menu = MENUS[i];
      if (menu.parentMenu == currentMenu) {
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
    Serial.println("wait");
  }
  char answer = NO_ORDER;
  radio.read(&answer, sizeof(answer));
  return answer;
}

int sendOrderAndWaitForInt(char order) {
  sendOrder(order);
  while (!radio.available()) {
    Serial.println("wait");
  }
  int answer = 0;
  radio.read(&answer, sizeof(answer));
  return answer;
}

float sendOrderAndWaitForFloat(char order) {
  sendOrder(order);
  while (!radio.available()) {
    Serial.println("wait");
  }
  float answer = 0;
  radio.read(&answer, sizeof(answer));
  return answer;
}

