/*! Created on Wed Nov 29 2023
    \author jcsb@pixmob.com
    \copyright jcsb@pixmob.com
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    \brief USB game controller firmware
    
*/

#include <Arduino.h>
// project modules
#include "bsp.h"
#include "74hc165.h"
#include "keyJoystick.h"
// submodules
// #include <Adafruit_MPU6050.h>
#include "tact.h"


// #define DEBUG_MAINlR                                                                                                                                                                                                               lRrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr
// WARNING: Do not put looped msgs will prevent flashing
// (Short top right pins 2 and 3 to reset in bootloader)

#ifndef DEBUG_MAIN
#define debugf_init()
#define debugf(msg)
#else
#define debugf_init() Serial.begin(9600);
#define debugf(msg) Serial.println(msg)
#endif


// =======================
// 1 enum pour idx de buttons (pas besoin chiffres)
// 1 list de pairs

/*! \brief This table maps to the Joystick button indexes on the HID device on the PC
  It also maps to the order of the local array buttons[] */
typedef enum {
BTN_IDX_A,
BTN_IDX_B,  

BTN_IDX_L,
BTN_IDX_L2,
BTN_IDX_R,
BTN_IDX_R2,

BTN_IDX_START,
BTN_IDX_SELECT,

BTN_IDX_C_UP,
BTN_IDX_C_DOWN,
BTN_IDX_C_LEFT,
BTN_IDX_C_RIGHT,

BTN_IDX_ARR_UP,
BTN_IDX_ARR_DOWN,
BTN_IDX_ARR_LEFT,
BTN_IDX_ARR_RIGHT,

// Local buttons (not transmitted via HID, configs the controller)
BTN_IDX_PLAYER,
} button_idx_t;

keyjoy_button_t button_keys[] = {
  {.id = BTN_IDX_A,           .ascii = ' '},
  {.id = BTN_IDX_B,           .ascii = KEY_KP_ENTER},
  {.id = BTN_IDX_L,           .ascii = 'l'},
  {.id = BTN_IDX_L2,          .ascii = 'L'},
  {.id = BTN_IDX_R,           .ascii = 'r'},
  {.id = BTN_IDX_R2,          .ascii = 'R'},
  {.id = BTN_IDX_START,       .ascii = KEY_ESC},
  {.id = BTN_IDX_SELECT,      .ascii = 'p'},
  {.id = BTN_IDX_ARR_UP,      .ascii = KEY_UP_ARROW},
  {.id = BTN_IDX_ARR_DOWN,    .ascii = KEY_RIGHT_ARROW},
  {.id = BTN_IDX_ARR_LEFT,    .ascii = KEY_LEFT_ARROW},
  {.id = BTN_IDX_ARR_RIGHT,   .ascii = KEY_DOWN_ARROW},
  {.id = BTN_IDX_C_UP,        .ascii = 'r'},
  {.id = BTN_IDX_C_DOWN,      .ascii = 'E'},
  {.id = BTN_IDX_C_LEFT,      .ascii = 'm'},
  {.id = BTN_IDX_C_RIGHT,     .ascii = KEY_TAB},

};


keyJoystick Keyjoy = keyJoystick(button_keys);


// Accelerometer
// =======================
// mpu6050 mpu;
// Adafruit_MPU6050 mpu;

// Buttons
// =======================
#define TACT_POLL_FREQ_HZ (100)
#define TACT_SR_LOGIC 0 // Shift register button logic
#define TACT_SHIFT_REG_NB 2
#define TACT_GPIO_LOGIC 0 // Buttons on GPIOs logic

shift165 shiftin = shift165(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, PIN_SHIFT_LATCH, TACT_SHIFT_REG_NB);

int shiftRead(int bit) { return shiftin.read(bit); }
inline int ioRead(int io) { return digitalRead((uint8_t)io); }

tact buttons[] = {
  // Buttons connected to MCU IOs
  tact(PIN_BTN_A, ioRead, TACT_POLL_FREQ_HZ, TACT_GPIO_LOGIC),
  tact(PIN_BTN_B, ioRead, TACT_POLL_FREQ_HZ, TACT_GPIO_LOGIC),
  // Buttons connected to shift register pins
  tact(SHIFT_BTN_L/* 0 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_L2/* 1 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_R/* 3 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_R2/* 4 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_START/* 6 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_SELECT/* 7 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_C_UP/* 8 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_C_DOWN/* 9 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_C_LEFT/* 10 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_C_RIGHT/* 11 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_ARR_UP/* 12 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_ARR_DOWN/* 13 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_ARR_LEFT/* 14 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(SHIFT_BTN_ARR_RIGHT/* 15 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  // Put config buttons last
  tact(SHIFT_BTN_PLAYER/* 2 */, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
};
const uint8_t nb_buttons = sizeof(buttons)/sizeof(tact);
const uint8_t nb_ioButtons = 2;
const uint8_t nb_configButtons = 1;

// Main
// =======================
void setup() {
  debugf_init();
  debugf("USB Gaming!");

  // Accelerometer
  // mpu.init();
  // wire.begin()
  // mpu.begin();

  // Buttons
  for (uint8_t i = 0; i < nb_buttons; i++) {
    buttons[i].setDebouncePeriod(30);
  }
  for (uint8_t i = 0; i < nb_ioButtons; i++) {
    pinMode(buttons[i].getPin(), INPUT);
  }

  // Analog Joystick
  pinMode(PIN_JOY_X, INPUT);
  pinMode(PIN_JOY_Y, INPUT);

  // HID Joystick
  #ifndef DEBUG_MAIN
  Keyjoy.begin();
  #endif
  
  // Leds
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);

  shiftin.init();
}

int iButt = 0; // Must be global, used in lambdas

// Color pulse
uint8_t color_idx = 0;
int8_t intensity = 0;
int8_t dir = 1;
uint8_t max_col_idx = 3;
uint8_t rgb[4][3] = {
  {0, 0, 0},
  {255, 0, 0},
  {0, 255, 0},
  {255, 255, 0}
};

void color_step()
{
  intensity+=dir;
  if (intensity >= 100 || !intensity) { dir = -dir; }
  float r = (rgb[color_idx][0] * intensity) / 100;
  float g = (rgb[color_idx][1] * intensity) / 100;
  float b = (rgb[color_idx][2] * intensity) / 100;
  analogWrite(PIN_LED_R, r);
  analogWrite(PIN_LED_G, g);
  analogWrite(PIN_LED_B, b);
}


void loop() {
  static unsigned long last_poll = 0;
  static unsigned int millisec_between_polls = 1000 / TACT_POLL_FREQ_HZ;

  if (millis() - last_poll >= millisec_between_polls) {
 
    color_step();
  
    Keyjoy.read_stick(PIN_JOY_X, PIN_JOY_Y);

    shiftin.capture();
    for (iButt = 0; iButt < (nb_buttons-nb_configButtons); iButt++) {
      #ifndef DEBUG_MAIN
      
      buttons[iButt].poll([] { Keyjoy.press(iButt);  },
                      [] { Keyjoy.release(iButt); },
                      0,
                      [] { Keyjoy.release(iButt); });
      
      #else
      buttons[iButt].poll([] {  debugf("Idx: " + String(iButt) + " Pin: " +  String(buttons[iButt].getPin()));  },
                      [] { debugf((char)button_keys[iButt].ascii); },
                      [] {  });
      #endif
    }


    // Last button in the index is a special button, not a HID button
    buttons[nb_buttons-1].poll([] { debugf("CONF: " + String(iButt) + " Pin: " +  String(buttons[iButt].getPin()));  },
                    [] { color_idx++; if(color_idx > max_col_idx) {color_idx = 0;} },
                    [] {
                      digitalWrite(PIN_LED_B, HIGH);
                      Keyjoy.begin();
                      digitalWrite(PIN_LED_B, LOW);
                    });
                          


    last_poll += millisec_between_polls;

    #ifndef DEBUG_MAIN
    Keyjoy.update();
    #endif
  }

  // poll buttons, joystick
  // read accelerometer if ON
  // leds if buttons pressed
  // rumble when accelerometer moves fast

  // switch for rumble, accelerometer
  // transistors for rumble 2
}
