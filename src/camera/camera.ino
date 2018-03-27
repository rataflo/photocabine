/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * Fast Read\Write : https://github.com/mmarchetti/DirectIO
 * 4* 7 segment, TM1637 : https://github.com/avishorp/TM1637
 * LCD Matrix 8*8 : http://wayoda.github.io/LedControl/
 */
#include <DirectIO.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include <LedControl.h>
#include "TM1637Display.h"
#include "constants.h"

/*
 * GLOBAL VARIABLES
 */
byte states[7] = {0, 0, 0, 0, 0, 0, 0}; // Step of differents photo.
byte arms[7] = {0, 0, 0, 0, 0, 0, 0}; // State of arms (0 = no photo, > 0 = number of the photo related to states[])

unsigned long currentMillis = 0;

// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
Input<MENU_BTN1_PIN> btnMenu1;
Input<MENU_BTN2_PIN> btnMenu2;
bool bMainScreen = true;
byte currentMenu = 0;
byte currentLineInMenu = 0;
unsigned long previousMenuMillis = 0;

// Flash
Output<FLASH_PIN> flash;

// Work variables
bool bGoTakeShot = false; 
byte stepTakeShot = 0;
bool bInitPhotomaton = false;
bool bFlashOn = false;
bool bStartLedOn = false;
bool bReadySpider = false; // true when spider is ok to receive a strip of paper

#include "scissor.h"
#include "shutter.h"
#include "coinAcceptor.h"
#include "tests.h"

void setup() {
  Serial.begin(9600);
  initPhotomaton();
  enableCoinAcceptor();
  showMainScreen();
}

void loop() {
  currentMillis = millis();

  // if price is OK, let's go.
  if(cents >= PRICE_CTS) {
    bGoTakeShot = true;
    stepTakeShot = 1;
    // disable coin acceptor for the duration of the shots.
    disableCoinAcceptor();
  }

  if(bGoTakeShot) {
    manageStepsTakeShot(); 
  }
  
  // if shot finished and coin acceptor disabled.
  if(!bGoTakeShot && !bCoinEnabled){
    enableCoinAcceptor();
  }
  
  refreshCoinSegment();
  checkMenu();
}

void manageStepsTakeShot(){
  switch (stepTakeShot) {
    case 1: // 5 sec wait between shots.
    case 4:
    case 7:
    case 10:
      showCountdown();
      break;
      
    case 2: // Take photo.
    case 5:
    case 8:
    case 11:
      takeShot();
      break;
      
    case 3: // move paper for next shot.
    case 6:
    case 9:
      movePaperNextShot();
      break;
    case 12:// last paper move, need to wait for spider to be at the correct position and scissor opened.  
      break;
      
    case 13: // Open scissor before last paper move
      openScissor();
      break;
      
    case 14: // cut paper and zou!
      closeScissor();
      break;
      
    case 15: // move paper back for first shot.
      movePaperFirstShot();
      bGoTakeShot = false; // Finish
      break;
  }
  stepTakeShot++;
}

/***************************
 *    INITS
 **************************/
void initPhotomaton(){

  // stepper shutter
  shutter.setMaxSpeed(1000);
  shutter.setAcceleration(400);
  shutter.moveTo(-200);
  
  // stepper scissor
  scissor.setMaxSpeed(1000);
  scissor.setAcceleration(400);
  scissor.moveTo(-200);
  
  // stepper papier
  pinMode(PAPER_PIN_STP, OUTPUT);
  pinMode(PAPER_PIN_DIR, OUTPUT); 
   
  // lcd
  lcd.begin();
  
  // Coin acceptor off
  pinMode(COIN_PIN, INPUT); 
  disableCoinAcceptor();
  
  // flash off
  flashOff();
  
  // Start button off
  startLedOff();
  
  // 4 * 7 segment display for coin
  initCoinSegment();
  
  // Led Matrix
  initLedMatrix();
  
  // Init steppers position.
  // Scissor
  lcd.setCursor(0,0);
  lcd.print("Scissor:");
  initScissor();
  lcd.print("OK");
  // Shutter
  lcd.print(" Shut:");
  initShutter();
  lcd.print("OK");

  //@TODO: wait for spider.
  
  delay(1000);
  bInitPhotomaton = true;
}


void flashOn() {
  flash.write(LOW);
  bFlashOn = true;
}

void flashOff() {
  flash.write(HIGH);
  bFlashOn = false;
}

/*****************
* MENU - GO PRO STYLE
******************/
/*
 * Check and do action on both menu button */
void checkMenu(){
  if (currentMillis - previousMenuMillis >= MENU_SPEED) {
    previousMenuMillis += MENU_SPEED;
    if(btnMenu1.read()){
      nextMenu();
    } else if(!bMainScreen && btnMenu2.read()){
      doMenu();
    }
  }
}

/*
 * Show main screen
 */
void showMainScreen(){
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
 * What to do on action button
 */
void doMenu(){
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
     takeShot();
     while(!bCloseShutter){
      shutter.run();
      checkShotFinished();
     }
      break;
    case 13: // Test 
      
      // close scissor.
      closeScissor(); 
      while(!bCloseScissor){
        scissor.run();
        checkScissorClosed();
      }
      
      // Open scissor
      openScissor();
      while(!bOpenScissor){
        scissor.run();
        checkScissorOpened();
      }
     
      // close again
      closeScissor(); // close scissor.
      while(!bCloseScissor){
        scissor.run();
        checkScissorClosed();
      }
      break;
     case 18: // Return menu tests
     case 23: // Return menu tests
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
      break;
      
     case 19: // Test switch shutter.
      delay(200);
      while(!btnMenu2.read()){
       testSwitchShutter();
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;
      
     case 20: // Test switch scissor.
      delay(200);
      while(!btnMenu2.read()){
       testSwitchScissor();
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;

    case 21: // Test switch up & down.
      delay(200);
      while(!btnMenu2.read()){
       testSwitchUpDown();
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;
    
    case 22: // Start button
      delay(200);
      while(!btnMenu2.read()){
       testStartButton();
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;
      
    case 24: // Test flash
      if(!bFlashOn){
        flashOn();
        lcd.setCursor(0, currentLineInMenu % 4);
        lcd.print(">Flash off          ");
      }else{
        flashOff();
        lcd.setCursor(0, currentLineInMenu % 4);
        printMsgToLCD(menu.label, true);
      }
      break;
      
    case 25: // Test led start button
      if(!bStartLedOn){
        startLedOn();
        lcd.setCursor(0, currentLineInMenu % 4);
        lcd.print(">Start LED off      ");
      }else{
        startLedOff();
        lcd.setCursor(0, currentLineInMenu % 4);
        printMsgToLCD(menu.label, true);
      }
      break;
      
    
      
    
  }
}

/*
 * Action on button next
 */
void nextMenu(){
  Serial.println("nextMenu");
  if(bMainScreen) { // if currently on main screen, show first menu.
    Serial.println("premier menu");
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



