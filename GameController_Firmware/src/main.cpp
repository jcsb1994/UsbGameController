/*! Created on Wed Nov 29 2023
    \author jcsb@pixmob.com
    \copyright jcsb@pixmob.com
    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    \brief USB game controller firmware
    
*/

#include <Arduino.h>
// project modules
#include "74hc165.h"
// submodules
// #include <Adafruit_MPU6050.h>
// #include <Joystick.h>
#include "tact.h"

#define DEBUG_MAIN
// WARNING: Do not put looped msgs will prevent flashing
// (Short top right pins 2 and 3 to reset in bootloader)

#ifndef DEBUG_MAIN
#define debugf_init()
#define debugf(msg)
#else
#define debugf_init() Serial.begin(9600);
#define debugf(msg) Serial.println(msg)
#endif


// Pins
// =======================
typedef enum {
  PIN_LED_R = 4,
  PIN_LED_G = 5,
  PIN_LED_B = 6,
  PIN_JOY_X = A3,
  PIN_JOY_Y = A2,
  
  PIN_BTN_A = 10,
  PIN_BTN_B = 16,

  PIN_SHIFT_DATA = 14,
  PIN_SHIFT_CLOCK = 9,
  PIN_SHIFT_LATCH = 8,
} pin_config_t;

// TODO: pins are wrong
typedef enum {
  SHIFT_BTN_L = 4,
  SHIFT_BTN_L2 = 5,  // Z on 64
  SHIFT_BTN_R = 6,
  SHIFT_BTN_R2 = 7,
  SHIFT_BTN_PLAYER = 14, // Local button, not part of USB joystick
  SHIFT_BTN_START = 8,
  SHIFT_BTN_SELECT = 9,
  SHIFT_BTN_C_UP = 10,
  SHIFT_BTN_C_DOWN = 11,
  SHIFT_BTN_C_LEFT = 12,
  SHIFT_BTN_C_RIGHT = 13,
  SHIFT_BTN_MAX
} shift_config_t;

// HID Joystick (USB)
// =======================
#ifndef DEBUG_MAIN
Joystick_ Joystick; // main USB device
#endif

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

// Shift Register Buttons
// =======================
shift165 shiftin = shift165(PIN_SHIFT_DATA, PIN_SHIFT_CLOCK, PIN_SHIFT_LATCH, TACT_SHIFT_REG_NB);
int shiftRead(int bit) { return shiftin.read(bit); }

tact shiftButtons[] = {
  tact(0, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(1, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(2, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(3, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(4, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(5, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(6, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(7, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(8, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(9, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(10, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(11, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(12, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(13, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(14, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
  tact(15, shiftRead, TACT_POLL_FREQ_HZ, TACT_SR_LOGIC),
};
const uint8_t nb_shiftButtons = sizeof(shiftButtons)/sizeof(tact);

inline int ioRead(int io) { return digitalRead((uint8_t)io); }
// GPIO Buttons
// =======================
tact ioButtons[] = {
  tact(PIN_BTN_A, ioRead, TACT_POLL_FREQ_HZ, TACT_GPIO_LOGIC),
  tact(PIN_BTN_B, ioRead, TACT_POLL_FREQ_HZ, TACT_GPIO_LOGIC),
};
const uint8_t nb_ioButtons = sizeof(ioButtons)/sizeof(tact);

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
  for (uint8_t i = 0; i < nb_ioButtons; i++) {
    ioButtons[i].setDebouncePeriod(30);
    pinMode(ioButtons[i].getPin(), INPUT);
  }
  for (uint8_t i = 0; i < nb_shiftButtons; i++) {
    shiftButtons[i].setDebouncePeriod(30);
  }

  // // Analog Joystick
  // pinMode(PIN_JOY_X, INPUT);
  // pinMode(PIN_JOY_Y, INPUT);

  // // HID Joystick
  // #ifndef DEBUG_MAIN
  // Joystick.begin(false);
  // #endif
  
  // Leds
  // pinMode(PIN_LED_R, OUTPUT);
  // pinMode(PIN_LED_G, OUTPUT);
  // pinMode(PIN_LED_B, OUTPUT);

}

int iButt = 0; // Must be global, used in lambdas
uint8_t intes = 0;
uint8_t idx = 4;
void loop() {
  static unsigned long last_poll = 0;
  static unsigned int millisec_between_polls = 1000 / TACT_POLL_FREQ_HZ;

  if (millis() - last_poll >= millisec_between_polls) {
  //   if (intes >= 48) {
  //     idx++;
  //     if (idx > 6) idx = 4;
  //   }
  //   analogWrite(idx, intes);
  //   intes++;
  
  //   #ifndef DEBUG_MAIN
  //   Joystick.setXAxis(analogRead(PIN_JOY_X));
  //   Joystick.setYAxis(analogRead(PIN_JOY_Y));
  //   #endif

    for (iButt = 0; iButt < nb_ioButtons; iButt++) {
      #ifndef DEBUG_MAIN
      ioButtons[iButt].poll([] { Joystick.setButton(iButt, 1);  },
                      [] { Joystick.setButton(iButt, 0); },
                      [] {  });
      #else
      ioButtons[iButt].poll([] {  debugf(iButt);  },
                      [] { debugf(iButt); },
                      [] {  });
      #endif
    }
  
    shiftin.capture();
    for (iButt = 0; iButt < nb_shiftButtons; iButt++) {
      #ifndef DEBUG_MAIN
      // TODO: DO NOT SET STATE FOR PLAYER BUTTON
      shiftButtons[iButt].poll([] { Joystick.setButton(iButt, 1);  },
                      [] { Joystick.setButton(iButt, 0); },
                      [] {  });
      #else
      shiftButtons[iButt].poll([] {  debugf(iButt);  },
                      [] { debugf(iButt); },
                      [] {  });
      #endif
    }

    last_poll += millisec_between_polls;

    #ifndef DEBUG_MAIN
    Joystick.sendState(); // Update USB state
    #endif
  }

  // poll buttons, joystick
  // read accelerometer if ON
  // leds if buttons pressed
  // rumble when accelerometer moves fast

  // switch for rumble, accelerometer
  // transistors for rumble 2
}
