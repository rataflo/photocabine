/*
 * LCD lib : https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library. Pin 20 SDA, 21 SCL. Adresse 0x27.
 * 
 */

#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include "constants.h"

/*
 * GLOBAL VARIABLES
 */
byte states[7] = {0, 0, 0, 0, 0, 0, 0}; // Step of differents photo.
byte arms[7] = {0, 0, 0, 0, 0, 0, 0}; // State of arms (0 = no photo, > 0 = number of the photo related to states[])

unsigned long currentMillis = 0;

// Shutter
AccelStepper shutter(1, SHUTTER_PIN_STP, SHUTTER_PIN_DIR);
unsigned long previousShotMillis = 0;
bool bCloseShutter = false; // Shutter is closed?

// Scissor.
AccelStepper scissor(1, SCISSOR_PIN_STP, SCISSOR_PIN_DIR);
unsigned long previousScissorMillis = 0;
int currentScissorNbStep = 200; // Number of current step for fully open scissor.
bool bCloseScissor = false; // True if scissor is closed.
bool bOpenScissor = true; // true if scissor wide open

// Paper
unsigned long previousPaperMillis = 0;
int currentPaperNbStep = 0;
unsigned int paperStep = HIGH; // Actual state of stepper.
bool bPaperShotOK = false; // True when paper is positionned to take a shot.
bool bPhotoTaken = false; // True when photo is taken.

// Spider up/down
unsigned long previousUpDownMillis = 0;

// Spider rotate
unsigned long previousRotateMillis = 0;
unsigned int spiderRotateStep = HIGH; // Actual state of stepper.
byte spiderState = 0; // 0 = not moving, 1 = down, 2 = up, 3 = rotate

// servo arm position
Servo servoArm;

bool bInitPhotomaton = false;


// LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
bool bMainScreen = true;
byte currentMenu = 0;
byte currentLineInMenu = 0;
unsigned long previousMenuMillis = 0;

// Coin acceptor
volatile int cents = 0;
bool bCoinEnabled = false;

bool bGoTakeShot = false; 
byte stepTakeShot = 0;

#include "tests.h"


void setup() {
  Serial.begin(9600);
  
  // stepper shutter
  pinMode(SHUTTER_ENDSTOP_PIN, INPUT);
  shutter.setMaxSpeed(1000);
  shutter.setAcceleration(400);
  shutter.moveTo(-200);
  
  // stepper scissor
  pinMode(SCISSOR_ENDSTOP_PIN, INPUT);
  scissor.setMaxSpeed(1000);
  scissor.setAcceleration(400);
  scissor.moveTo(-200);
  
  // stepper papier
  pinMode(PAPER_PIN_STP, OUTPUT);
  pinMode(PAPER_PIN_DIR, OUTPUT); 
  
  // DC motor spider up/down
  pinMode(SPIDER_UPDOWN_PIN_UP, OUTPUT); 
  digitalWrite(SPIDER_UPDOWN_PIN_UP,HIGH); // HIGH is off...
  pinMode(SPIDER_UPDOWN_PIN_DOWN, OUTPUT);
  digitalWrite(SPIDER_UPDOWN_PIN_DOWN, HIGH);
  pinMode(SPIDER_UPDOWN_ENDSTOP_PIN, INPUT); 
  
  // stepper spider rotation
  pinMode(SPIDER_ROTATE_PIN_STP, OUTPUT);
  pinMode(SPIDER_ROTATE_PIN_DIR, OUTPUT); 

  // servo arm position
  servoArm.attach(SERVO_ARM);
  servoArm.write(180);  
  
  // lcd
  pinMode(MENU_BTN1_PIN, INPUT);
  pinMode(MENU_BTN2_PIN, INPUT);
  lcd.begin();

  // Coin acceptor
  pinMode(ENABLE_COIN_PIN, OUTPUT); 
  disableCoinAcceptor();
  pinMode(COIN_PIN, INPUT); 
  attachInterrupt(digitalPinToInterrupt(COIN_PIN), coinInterrupt, RISING);
  
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

  // run steppers.
  
  /*digitalWrite(SPIDER_UPDOWN_PIN_UP,LOW); //on for 2 second.
  delay(2000);
  digitalWrite(SPIDER_UPDOWN_PIN_UP,HIGH); //off
  delay(500);//delay beteen switching relay to avoid shortcut.
  
  digitalWrite(SPIDER_UPDOWN_PIN_DOWN, LOW);//on for 2 second
  delay(2000);
  digitalWrite(SPIDER_UPDOWN_PIN_DOWN,HIGH); //off
  delay(500);//delay beteen switching relay to avoid shortcut.*/
  
  checkMenu();
  // Premier démarrage, init du bouzin.
  /*if(!bInitPhotomaton){
    initPhotomaton();
  } else if(bMoneyOK){
    takeShot();
  }

  shutter.run();
  scissor.run();*/
  /*
  checkForMoney();

  // Check which step need to be done.
  bool bNeedTakePhoto = false;
  bool bNeedMovePaperOneShot = false;
  bool bNeedEndMovePaper = false;
  bool bNeedScissor = false;
  bool bNeedMoveSpider = false;
  for(int i = 0; i < 7; i++){
    if(states[i] > 0) {
      if(states[i] < 5) {
        if(bPaperShotOK) {
          bNeedTakePhoto = true;
        } else {
          bNeedMovePaperOneShot = true;
        }
      } else if (states[i] == 5) { // Move paper down
        bNeedEndMovePaper = true;
      } else if(states[i] == 6) { // cut paper.
        bNeedScissor = true;
      } else { // Moving paper in chemical tray.
        bNeedMoveSpider = true;
      }
    }
  }

  if(bNeedTakePhoto){
    takePhoto();
  }

  if(bNeedMovePaperOneShot){
    movePaperOneShot();
  }

  if(bNeedEndMovePaper){
    
  }

  if(bNeedScissor){
    if(!bCloseScissor) { 
      closeScissor();
    } else {
      openScissor();
    }
  }

  if(bNeedMoveSpider){
    moveSpider();
  }

  // Update states
  for(int i = 0; i < 7; i++){
    if(states[i] > 0) {
      if(states[i] < 5 && bPhotoTaken) {
        states[i]++;
        bPhotoTaken = false;
      }
    }
  }*/
}

void manageStepsTakeShot(){
  switch (stepTakeShot) {
    case 1: // 5 sec wait between shots.
    case 4:
    case 7:
    case 10:
      if (currentMillis - previousShotMillis >= WAIT_BETWEEN_SHOT) {
        stepTakeShot++;
      }
      break;
      
    case 2: // Take photo.
    case 5:
    case 8:
    case 11:
      if(bCloseShutter) { 
        takeShot();
      } else {
        checkShotFinished();
      }
      break;
      
    case 3: // move paper for next shot.
    case 6:
    case 9:
    case 12:// last paper move, need to wait for spider to be at the correct position and scissor opened.  
      break;
      
    case 13: // Open scissor before last paper move
      if(!bOpenScissor) { 
        openScissor();
      } else {
        checkScissorOpened();
        if(bOpenScissor) {
          stepTakeShot++;
        }
      }
      break;
      
    case 14: // cut paper and zou!
      if(!bCloseScissor) { 
        closeScissor();
      } else {
        checkScissorClosed();
        if(bCloseScissor) {
          stepTakeShot++;
        }
      }
      break;
      
    case 15: // move paper back for first shot.
      break;

    
  }
}

void openScissor() {
  if(scissor.currentPosition() > SCISSOR_STEP_OPENED){
    scissor.moveTo(SCISSOR_STEP_OPENED);
    bCloseScissor = false;
  }
}

void checkScissorOpened() {
  bOpenScissor = scissor.currentPosition() == SCISSOR_STEP_OPENED;
  bCloseScissor = !bOpenScissor;
}

void closeScissor() {
  if(!bCloseScissor){
    scissor.moveTo(0);
    bOpenScissor = false;
  }
}

void checkScissorClosed() {
  bCloseScissor = false;
  if(scissor.currentPosition() > -5) {
    bCloseScissor = !digitalRead(SCISSOR_ENDSTOP_PIN);
  }
  bOpenScissor = !bCloseScissor;
}


void takeShot() {
  // TODO calculate duration of the shot.
  shutter.moveTo(-201);
  bCloseShutter = false;
}

void checkShotFinished() {
  boolean bEndStop = false;
  bCloseShutter = false;
  int currentShutterNbStep = shutter.currentPosition();
  if(currentShutterNbStep < -195){ // Digital read is time consuming so we use it at the last steps.
    bEndStop = !digitalRead(SHUTTER_ENDSTOP_PIN);
  }

  if(bEndStop){
    shutter.stop();
    shutter.run();
    shutter.setCurrentPosition(0);
    bCloseShutter = true;
    stepTakeShot++;
    previousShotMillis = currentMillis;
  }
}


void movePaperOneShot() {
  if(currentPaperNbStep < NB_STEP_ONE_SHOT){
    if (currentMillis - previousPaperMillis >= PAPER_STEPPER_SPEED) {
      digitalWrite(SCISSOR_PIN_DIR, LOW); // Voir si bonne direction
      paperStep = paperStep == HIGH ? LOW : HIGH;
      digitalWrite(PAPER_PIN_STP, paperStep);
      previousPaperMillis += PAPER_STEPPER_SPEED;
      currentPaperNbStep++;
      bPaperShotOK = false;
    }
  } else {
    bPaperShotOK = true;
  }
}

/*
 * Move Spider.
 */
void moveSpider() {
  switch (spiderState) {
    case 1:
      downSpider();
      break;
    case 2:
      upSpider();
      break;
    case 3: 
      rotateSpider();
      break;
  }
}

void downSpider(){
}

void upSpider(){
}

void rotateSpider(){
}

/***************************
 *    INITS
 **************************/
void initPhotomaton(){

  // Init steppers (blocking)
  // Scissor
  lcd.setCursor(0,1);
  lcd.print("Init Scissor:");
  initScissor();
  lcd.print("OK");
  // Shutter
  lcd.setCursor(0,2);
  lcd.print("Init Shutter:");
  initShutter();
  lcd.print("OK");
  // Spider

  
  delay(1000);
  bInitPhotomaton = true;
}

boolean initSpider() {
  
}

void initShutter() {
  boolean bEndStop = !digitalRead(SHUTTER_ENDSTOP_PIN);
  while (!bEndStop) { 
    shutter.moveTo(shutter.currentPosition() - 1); 
    shutter.run();
    bEndStop = !digitalRead(SHUTTER_ENDSTOP_PIN);
  }
  shutter.setCurrentPosition(0);
  bCloseShutter = true;
}

void initScissor() {
  boolean bEndStop = !digitalRead(SCISSOR_ENDSTOP_PIN);
  while (!bEndStop) { 
    scissor.moveTo(scissor.currentPosition() + 1); 
    scissor.run();
    bEndStop = !digitalRead(SCISSOR_ENDSTOP_PIN);
  }
  scissor.setCurrentPosition(0);
  bCloseScissor = true;
  bOpenScissor = false;
}


/*****************
* MENU - GO PRO STYLE
******************/
/*
 * Check and do action on both menu button */
void checkMenu(){
  if (currentMillis - previousMenuMillis >= MENU_SPEED) {
    previousMenuMillis += MENU_SPEED;
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
  bMainScreen = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-- MAIN SCREEN --");
}

void printMsgToLCD(String message, bool isSelected){
  String tmp;
  Serial.println(tmp);
  if(isSelected){
    tmp = sel;
  } else {
    tmp = espace;
  }
  Serial.println(tmp);
  tmp+= message;
  Serial.println(tmp);
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
      Serial.println(menu.label);
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
Serial.println(ligneLCD);
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
      currentMenu = 5;
      currentLineInMenu = 0;
      showMenu();
      break;
      
     case 19: // Test switch shutter.
      delay(200);
      while(!digitalRead(MENU_BTN2_PIN)){
       testSwitchShutter();
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;
      
     case 20: // Test switch scissor.
      delay(200);
      while(!digitalRead(MENU_BTN2_PIN)){
       testSwitchScissor();
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
      break;

    case 21: // Test switch up & down.
      delay(200);
      while(!digitalRead(MENU_BTN2_PIN)){
       testSwitchUpDown();
       delay(200);
      }
      lcd.setCursor(0, currentLineInMenu % 4);
      printMsgToLCD(menu.label, true);
      delay(200);
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

/*
 *  COIN ACCEPTOR
 */
// interrupt main loop each time a pulse from coin acceptor is coming.
// 1 pulse = 10cts
void coinInterrupt(){
  cents += bCoinEnabled ? 10 : 0;
}

void disableCoinAcceptor(){
  detachInterrupt(digitalPinToInterrupt(COIN_PIN));
  digitalWrite(SPIDER_UPDOWN_PIN_DOWN, LOW);
  bCoinEnabled = false;
}

void enableCoinAcceptor(){
  digitalWrite(SPIDER_UPDOWN_PIN_DOWN, HIGH);
  attachInterrupt(digitalPinToInterrupt(COIN_PIN), coinInterrupt, RISING);
  bCoinEnabled = true;
}

