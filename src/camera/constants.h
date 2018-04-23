// X: 22 / 23 
// Y: 24 / 25
// Z: 30 / 31
// Ext: 26 / 27

#define SHUTTER_PIN_STP 23
#define SHUTTER_PIN_DIR 22
#define SHUTTER_ENDSTOP_PIN 53

#define LED_MATRIX_SDI_PIN 12 // SDI = DIN
#define LED_MATRIX_CS_PIN 11 // CS
#define LED_MATRIX_SCL_PIN 10 // SCL = CLOCK

#define SCISSOR_PIN_STP 25
#define SCISSOR_PIN_DIR 24
#define SCISSOR_ENDSTOP_PIN 52
#define SCISSOR_STEP_OPENED 1000 // Number of step to fully open the scissor.

#define PAPER_PIN_STP 27
#define PAPER_PIN_DIR 26
#define NB_STEP_PAPER_ONE_SHOT 730 // Number of step to move to another shot.
#define NB_STEP_PAPER_OUT 3230 // Number of step to move out paper.
#define PAPER_SPEED 500
#define PAPER_ACCEL 500
#define DELTA_FIRST_SHOT -80 //Delta in step to do after paper reach opto 1.
#define PAPER_SWITCH1_PIN 48
#define PAPER_SWITCH2_PIN 49
#define PAPER_SWITCH3_PIN 50
#define PAPER_SWITCH4_PIN 51

#define FLASH_PIN 46

#define START_BTN_PIN 45
#define LED_START_BTN_PIN 47

#define MENU_BTN1_PIN 40
#define MENU_BTN2_PIN 41
#define MENU_SPEED 200 // check button menu each 200ms.

#define COIN_PIN 3 // Add 10k/100k pull up resistor on pin to 5V.
#define ENABLE_COIN_PIN 44 // If High coin acceptor is enable. LOW coin acceptor reject coins.
#define PRICE_CTS 200 // Price of a photo in cents.
#define COIN_SEGMENT_CLK_PIN 6
#define COIN_SEGMENT_DIO_PIN 7

#define WAIT_BETWEEN_SHOT 5000 // Wait between shot in ms.

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
  {7, "Steppers           ", 5},
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

// For LED Matrix (5, 4 ,3, 2, 1, smiley)
const byte IMAGES[][8] = {
{
  B00000000,
  B01100110,
  B01100110,
  B00000000,
  B10000001,
  B01000010,
  B00111100,
  B00000000
},
{
  B00000000,
  B00011000,
  B00011000,
  B00111000,
  B00011000,
  B00011000,
  B00011000,
  B01111110
},
{
  B00000000,
  B00111100,
  B01100110,
  B00000110,
  B00001100,
  B00110000,
  B01100000,
  B01111110
},
{
  B00000000,
  B00111100,
  B01100110,
  B00000110,
  B00011100,
  B00000110,
  B01100110,
  B00111100
},
{
  B00000000,
  B00001100,
  B00011100,
  B00101100,
  B01001100,
  B01111110,
  B00001100,
  B00001100
},
{
  B00000000,
  B01111110,
  B01100000,
  B01111100,
  B00000110,
  B00000110,
  B01100110,
  B00111100
}};

const byte ARROWDOWN[8] = {
  B00000000,
  B00011100,
  B00011100,
  B00011100,
  B01111111,
  B00111110,
  B00011100,
  B00001000
};

const int IMAGES_LEN = sizeof(IMAGES)/8;

