
#ifndef BSP_H
#define BSP_H

#include <Arduino.h>

// Pins
// =======================
typedef enum {
  PIN_LED_R = 5,
  PIN_LED_G = 6,
  PIN_LED_B = 4,
  PIN_JOY_X = A3,
  PIN_JOY_Y = A2,
  
  PIN_BTN_A = 10,
  PIN_BTN_B = 16,

  PIN_SHIFT_DATA = 14,
  PIN_SHIFT_CLOCK = 9,
  PIN_SHIFT_LATCH = 8,
} pin_config_t;



// Shift register pins
// =======================
typedef enum {
  SHIFT_BTN_L = 0,
  SHIFT_BTN_L2 = 0,  // Z on 64
  SHIFT_BTN_R = 0,
  SHIFT_BTN_R2 = 9,
  SHIFT_BTN_PLAYER = 2, // Local button, not part of USB joystick
  SHIFT_BTN_START = 3,  //0x10
  SHIFT_BTN_SELECT = 4, // 0x08
  SHIFT_BTN_C_UP = 10,
  SHIFT_BTN_C_DOWN = 11, // 0x200
  SHIFT_BTN_C_LEFT = 12, // 0x1000
  SHIFT_BTN_C_RIGHT = 13, // 0x800
  SHIFT_BTN_ARR_UP = 0,   // 0x400
  SHIFT_BTN_ARR_DOWN = 5,
  SHIFT_BTN_ARR_LEFT = 6,
  SHIFT_BTN_ARR_RIGHT = 7,
  SHIFT_BTN_MAX
} shift_config_t;

#endif