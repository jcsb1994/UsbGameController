#ifndef KEY_JOYSTICK_H
#define KEY_JOYSTICK_H

#include <Arduino.h>
#include <Joystick.h>
#include <Keyboard.h>

typedef enum {
    KEYJOY_STATE_UNINIT,
    KEYJOY_STATE_JOYSTICK,
    KEYJOY_STATE_KEYBOARD,
} keyjoy_state_t;

typedef struct {
    uint8_t id;
    unsigned char ascii;
} keyjoy_button_t;

class keyJoystick
{
private:
    keyjoy_state_t _state;
    keyjoy_button_t *_buttons;
    Joystick_ Joystick;

public:
    keyJoystick(keyjoy_button_t *buttons) : _buttons(buttons) {}
    ~keyJoystick() {}

    int begin();
    void press(uint8_t button);
    void release(uint8_t button);

    void read_stick(uint8_t x_pin, uint8_t y_pin);

    void update();
};

#endif