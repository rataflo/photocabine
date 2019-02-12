/*
 * SDA A4
 * SCL A5
 */

#include <LiquidCrystal_I2C.h>

#define MENU_BTN1_PIN 5
#define MENU_BTN2_PIN 6
#define MENU_SPEED 150 // check button menu each 200ms.

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
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
}menuItem;

#define TAILLE_MENU 25
const menuItem MENUS[TAILLE_MENU] = {
  {1, "...                ", 0},
  {2, "Pause              ", 0},
  {3, "Take shot          ", 0},
  {4, "Setup              ", 0},
  {5, "Tests              ", 0},
  {6, "...                ", 5},
  {7, "Motors             ", 5},
  {8, "Microswitchs       ", 5},
  {9, "LED                ", 5},
  {10,"Relay              ", 5},
  {11,"...                ", 7},
  {12,"Shutter            ", 7},
  {13,"Scissor            ", 7},
  {14,"Paper feeder       ", 7},
  {15,"Spider up/down     ", 7},
  {16,"Spider rotate      ", 7},
  {17,"Paper exit         ", 7},
  {18,"...                ", 8},
  {19,"Switch shutter     ", 8},
  {20,"Switch scissor     ", 8},
  {21,"Switch up&down     ", 8},
  {22,"Start              ", 8},
  {23,"...                ", 9},
  {24,"Flash on           ", 9},
  {25,"Start LED on       ", 9},
};

String sel = ">";
String espace = " ";

void setup() {
  Serial.begin(9600);
  pinMode(MENU_BTN1_PIN, INPUT_PULLUP);
  pinMode(MENU_BTN2_PIN, INPUT_PULLUP);
  initRemote();
  showMainScreen();
}

void loop() {
  
  checkMenu();
}

/*****************
* MENU - GO PRO STYLE
******************/

void initRemote(){
  lcd.begin();
}

/* Check and do action on both menu button */
void checkMenu(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMenuMillis >= MENU_SPEED) {
    previousMenuMillis = currentMillis;
    if(digitalRead(MENU_BTN1_PIN)){
      nextMenu();
    } else if(!bMainScreen && digitalRead(MENU_BTN2_PIN)){
      doMenu();
    }
  }
}

/*
 * Show main screen
 */
void showMainScreen(){
  Serial.println("main");
  bMainScreen = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-- MAIN SCREEN --");
}

void printMsgToLCD(String message, bool isSelected){
  String tmp;
  if(isSelected){
    tmp = sel;
  } else {
    tmp = espace;
  }
  tmp+= message;
  lcd.print(tmp);
}

/*
 * Display a menu.
 */
void showMenu(){
  int ligneLCD = 0;
  int lineInMenu = 0;
  
  for(int i = 0; i < TAILLE_MENU; i++){
    menuItem menu = MENUS[i];
    if(menu.parentMenu == currentMenu){
      if(lineInMenu >= currentLineInMenu) {
        lcd.setCursor(0, ligneLCD);
        printMsgToLCD(menu.label, lineInMenu == currentLineInMenu);
        ligneLCD++;
        if(ligneLCD == 4){
          break;
        }
      }
      lineInMenu++;
    }
  }
  
  for(int i = ligneLCD; i < 4; i++){
    lcd.setCursor(0, i);
    lcd.print("                    ");
  }
}

/*
 * Action on button next
 */
void nextMenu(){
  Serial.println("nextMenu");
  if(bMainScreen) { // if currently on main screen, show first menu.
    bMainScreen = false;
    currentMenu = 0;
    currentLineInMenu = 0;
    showMenu();
    
  } else {
    // Get number of items in menu.
    int nbItemMenu = 0;
    for(int i = 0; i < TAILLE_MENU; i++){
      menuItem menu = MENUS[i];
      if(menu.parentMenu == currentMenu){
        nbItemMenu++;
      }
    }

    // Case end of menu, return to first line.
    if(currentLineInMenu + 1  >= nbItemMenu){
      currentLineInMenu =  0;
      showMenu();
      
    } else if(currentLineInMenu == 3 || currentLineInMenu == 7 || currentLineInMenu == 11){ // Case show next page in menu
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

/*
 * What to do on action button
 */
void doMenu(){
  Serial.println("doMenu");
  // Get the menu in action
  int line = 0;
  menuItem menu;
  for(int i = 0; i < TAILLE_MENU; i++){
    menu = MENUS[i];
    if(menu.parentMenu == currentMenu){
      if(line == currentLineInMenu){
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
    case 2: // Pause
      break;
    case 3: // Take shot
    case 4: // Setup
    case 5: // Tests
      currentMenu = menu.id;
      currentLineInMenu = 0;
      showMenu();
      break;
    case 6: // Return 0
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
      delay(200);
      while(!digitalRead(MENU_BTN2_PIN)){
       //testSwitchShutter(lcd, currentLineInMenu);
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;
      
     case 20: // Test switch scissor.
      delay(200);
      while(!digitalRead(MENU_BTN2_PIN)){
       //testSwitchScissor(lcd, currentLineInMenu);
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;

    case 21: // Test switch up & down.
      delay(200);
      while(!digitalRead(MENU_BTN2_PIN)){
       //testSwitchUpDown(lcd, currentLineInMenu);
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;
    
    case 22: // Start button
      delay(200);
      while(!digitalRead(MENU_BTN2_PIN)){
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
