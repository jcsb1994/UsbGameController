#include <Arduino.h>
#include <Joystick.h>
// project features
#include "74hc165.h"
#include "mpu6050.h"
// submodules
#include "tact.h"

#if DEBUG_MAIN
#define debugf(msg) Serial.println(msg)
#else
#define debugf(msg)
#endif
// typedef enum {
//   PIN_BUTTON_A = 2,
//   PIN_BUTTON_B = 3,
//   PIN_BUTTON_L = 4,
//   PIN_BUTTON_L2 = 5,  // Z on 64
//   PIN_BUTTON_R = 6,
//   PIN_BUTTON_R2 = 7,
//   PIN_BUTTON_START = 8,
//   PIN_BUTTON_SELECT = 9,
//   PIN_BUTTON_C_UP = 10,
//   PIN_BUTTON_C_DOWN = 11,
//   PIN_BUTTON_C_LEFT = 12,
//   PIN_BUTTON_C_RIGHT = 13,
//   PIN_BUTTON_MAX
// } button_pin_t;

//  Buttons
#define TACT_POLL_FREQ_HZ (100)
#define TACT_ACTIVE_STATE 0

Joystick_ Joystick; // main USB device

#define NB_165 1
#define SHIFT_BUTTONS_NB (NB_165*8)
shift165 shiftin = shift165(2, 3, 4);
// int buttonRead(int bit) { return shiftin.read(bit); }
mpu6050 mpu;

// tact shiftButtons[SHIFT_BUTTONS_NB]; // FIXME: lib doit supporter ctor sans rien
// FIXME: enable release after long press doit aussi etre dans update lib
// FIXME: lib debounce doit etre moins de 100 par defaut, et passable par le ctor (avec dflt)
// FIXME: lib cb uint8_t pr fit avec arduino
tact shiftButtons[10] = {
  tact(5, digitalRead, TACT_POLL_FREQ_HZ, TACT_ACTIVE_STATE),
  tact(6, digitalRead, TACT_POLL_FREQ_HZ, TACT_ACTIVE_STATE),
  tact(7, digitalRead, TACT_POLL_FREQ_HZ, TACT_ACTIVE_STATE),
  tact(8, digitalRead, TACT_POLL_FREQ_HZ, TACT_ACTIVE_STATE),
  tact(9, digitalRead, TACT_POLL_FREQ_HZ, TACT_ACTIVE_STATE),
  tact(10, digitalRead, TACT_POLL_FREQ_HZ, 1),
  tact(16, digitalRead, TACT_POLL_FREQ_HZ, 1),
  tact(15, digitalRead, TACT_POLL_FREQ_HZ, 1),
  tact(14, digitalRead, TACT_POLL_FREQ_HZ, 1),
  tact(18, digitalRead, TACT_POLL_FREQ_HZ, 1)
};

// mpu6050 accelerometer;

void setup() {
  // Serial.begin(9600);
  // Serial.println("hello");
  // for (int i = 05; i < 5+SHIFT_BUTTONS_NB; i++) {
  //   shiftButtons[i] = tact(i, /* buttonRead */digitalRead, TACT_POLL_FREQ_HZ, TACT_ACTIVE_STATE);
  // }
  mpu.init();

  for (uint8_t i = 0; i < 10; i++)
  {
    shiftButtons[i].setDebouncePeriod(30);
  }

  Joystick.begin();
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);

}

int iButt = 0;
void loop() {
    static unsigned long last_poll = 0;
  static unsigned int millisec_between_polls = 1000 / TACT_POLL_FREQ_HZ;

  if (millis() - last_poll >= millisec_between_polls) {
    // TODO AUTOset state should be false, then at end of if, could send everything at once cuz we poll all anyway
    Joystick.setXAxis(analogRead(A1));
    Joystick.setYAxis(analogRead(A2));
    // shiftin.capture();
    for (iButt = 0; iButt < 10; iButt++) {
      shiftButtons[iButt].poll([] {  /* Serial.println("ye"); */Joystick.setButton(iButt, 1);  },
                      [] { /* Serial.println("ya"); */Joystick.setButton(iButt, 0); },
                      [] {  });
    }
    last_poll += millisec_between_polls;
  }

  // poll buttons, joystick
  // read accelerometer if ON
  // leds if buttons pressed
  // rumble when accelerometer moves fast

  // switch for rumble, accelerometer
  // transistors for rumble 2
}