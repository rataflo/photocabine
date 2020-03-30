#define RADIO_CSN 8
#define RADIO_CE 7
#define RADIO_SCK 13
#define RADIO_MOSI 11
#define RADIO_MISO 12

#define MENU_BTN1_PIN 5
#define MENU_BTN2_PIN 6
#define MENU_SPEED 150 // check button menu each 200ms.

// Define menu structure.
typedef struct
{
  byte id;
  byte parentMenu;
} menuItem;

#define TAILLE_MENU 44
const char sel = '>';
const char espace = ' ';

const menuItem MENUS[TAILLE_MENU]  = {
  {1, 0}, // MENU_RETOUR
  {2, 0}, // Pause
  {3, 0}, // Take shot
  {4, 0}, // Setup
  {5, 0}, // Start tests
  {6, 5}, // MENU_RETOUR
  {7, 5}, // Motors
  {8, 5}, // Empty
  {9, 5}, // LEDs
  {10, 5},// Simul
  {11, 7},// MENU_RETOUR
  {12, 7},// Shutter
  {13, 7},// Scissor
  {14, 7},// Paper
  {15, 7},// Spider up
  {16, 7},// Spider down
  {17, 7},// Rotate
  {18, 7},// Paper exit
  {19, 7},// Open arm
  {20, 7},// Close arm
  {21, 7},// Idle arm
  {22, 3},// MENU_RETOUR
  {23, 3},// Normal
  {24, 3},// Double exposure
  {25, 3},// Light painting
  {26, 3},// EMPTY
  {27, 3},// EMPTY
  {28, 3},// EMPTY
  {29, 3},// EMPTY
  {30, 3},// EMPTY
  {31, 3},// EMPTY
  {32, 3},// EMPTY
  {33, 3},// EMPTY
  {34, 9},// MENU_RETOUR
  {35, 9},// Flash
  {36, 9},// Start
  {37, 9},// Strip
  {38, 4},// MENU_RETOUR
  {39, 4},// Mode
  {40, 4},// Price
  {41, 4},// Free price
  {42, 4},// Tank time
  {43, 10},// MENU_RETOUR
  {44, 10}// Paper process
};

const char menu1[] PROGMEM = "^ Main menu";         // 1
const char menu2[] PROGMEM = "Pause";               // 2
const char menu3[] PROGMEM =  "Take shot";          // 3
const char menu4[] PROGMEM =  "Setup";              // 4
const char menu5[] PROGMEM =  "Start tests";        // 5
const char menu6[] PROGMEM =  "^ Tests";            // 6
const char menu7[] PROGMEM =  "Motors";             // 7
const char menu8[] PROGMEM =  "";                   // 8
const char menu9[] PROGMEM =  "LEDs";               // 9
const char menu10[] PROGMEM =  "Simulations";       // 10
const char menu11[] PROGMEM =  "^ Tests motors";    // 11
const char menu12[] PROGMEM =  "Shutter";           // 12
const char menu13[] PROGMEM =  "Scissor";           // 13
const char menu14[] PROGMEM =  "Paper";             // 14
const char menu15[] PROGMEM =  "Spider up";         // 15
const char menu16[] PROGMEM =  "Spider down";       // 16
const char menu17[] PROGMEM =  "Rotate";            // 17
const char menu18[] PROGMEM =  "Paper exit";        // 18
const char menu19[] PROGMEM =  "Open arm";          // 19
const char menu20[] PROGMEM =  "Close arm";         // 20
const char menu21[] PROGMEM =  "Idle arm";          // 21
const char menu22[] PROGMEM =  "^ Take shot";       // 22
const char menu23[] PROGMEM =  "Normal";            // 23
const char menu24[] PROGMEM =  "Double exposure";   // 24
const char menu25[] PROGMEM =  "Light painting";    // 25
const char menu26[] PROGMEM =  "";                  // 26
const char menu27[] PROGMEM =  "";                  // 27
const char menu28[] PROGMEM =  "";                  // 28
const char menu29[] PROGMEM =  "";                  // 29
const char menu30[] PROGMEM =  "";                  // 30
const char menu31[] PROGMEM =  "";                  // 31
const char menu32[] PROGMEM =  "";                  // 32
const char menu33[] PROGMEM =  "";                  // 33
const char menu34[] PROGMEM =  "^ Tests LEDs";      // 34
const char menu35[] PROGMEM =  "Flash";             // 35
const char menu36[] PROGMEM =  "Start btn";         // 36
const char menu37[] PROGMEM =  "Strip";             // 37
const char menu38[] PROGMEM =  "^ Setup";           // 38
const char menu39[] PROGMEM =  "Mode";              // 39
const char menu40[] PROGMEM =  "Price";             // 40
const char menu41[] PROGMEM =  "Free price";        // 41
const char menu42[] PROGMEM =  "Tank time";         // 42
const char menu43[] PROGMEM =  "^ Simulations";     // 43
const char menu44[] PROGMEM =  "Paper process";     // 44

PGM_P const LABELS[] PROGMEM = {
  menu1, 
  menu2, 
  menu3, 
  menu4, 
  menu5, 
  menu6, 
  menu7, 
  menu8, 
  menu9, 
  menu10,
  menu11, 
  menu12, 
  menu13, 
  menu14, 
  menu15, 
  menu16, 
  menu17, 
  menu18, 
  menu19, 
  menu20,
  menu21, 
  menu22, 
  menu23, 
  menu24, 
  menu25, 
  menu26, 
  menu27, 
  menu28, 
  menu29, 
  menu30, 
  menu31, 
  menu32, 
  menu33, 
  menu34, 
  menu35, 
  menu36, 
  menu37, 
  menu38, 
  menu39, 
  menu40, 
  menu41, 
  menu42, 
  menu43, 
  menu44
};


#define MODE_PAYING 0
#define MODE_FREE_PRICE 1
#define MODE_FREE 2
