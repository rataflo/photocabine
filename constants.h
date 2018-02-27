// X: 22 / 23 
// Y: 24 / 25
// Z: 30 / 31
// Ext: 26 / 27

#define SHUTTER_PIN_STP 30
#define SHUTTER_PIN_DIR 31
#define SHUTTER_ENDSTOP_PIN 53
#define SHUTTER_STEPPER_SPEED 8


#define SCISSOR_PIN_STP 22
#define SCISSOR_PIN_DIR 23
#define SCISSOR_ENDSTOP_PIN 52
#define SCISSOR_STEPPER_SPEED 2
#define SCISSOR_STEP_OPENED -100 // Number of step to fully open the scissor.

#define PAPER_PIN_STP 24
#define PAPER_PIN_DIR 25
#define PAPER_STEPPER_SPEED 3
#define NB_STEP_ONE_SHOT 100 // Number of step to move to another shot.

#define SPIDER_UPDOWN_PIN_UP 45
#define SPIDER_UPDOWN_PIN_DOWN 44
#define SPIDER_UPDOWN_ENDSTOP_PIN 48
#define SPIDER_UPDOWN_STEPPER_SPEED 10

#define SPIDER_ROTATE_PIN_STP 32
#define SPIDER_ROTATE_PIN_DIR 33
#define SPIDER_ROTATE_ENDSTOP_PIN 36
#define SPIDER_ROTATE_STEPPER_SPEED 10

#define SERVO_ARM 46

#define MENU_BTN1_PIN 50
#define MENU_BTN2_PIN 51
#define MENU_SPEED 200 // check button menu each 200ms (digitalSerialRead is time consuming).

#define COIN_PIN 3 // Add 10k/100k pull up resistor on pin to 5V.
#define ENABLE_COIN_PIN 5 // If High coin acceptor is enable. LOW coin acceptor reject coins.
#define PRICE_CTS 200 // Price of a photo in cents.

#define WAIT_BETWEEN_SHOT 5000 // Wait between shot in ms.

// Define menu structure.
typedef struct
{
    byte id; 
    String label; 
    byte parentMenu;
}menuItem;

#define TAILLE_MENU 21
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
};

String sel = ">";
String espace = " ";

