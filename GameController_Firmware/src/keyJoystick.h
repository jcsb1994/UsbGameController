#ifndef KEY_JOYSTICK_H
#define KEY_JOYSTICK_H

#include <Arduino.h>
#include <Joystick.h>
#include <Keyboard.h>

/*! \brief Can toggle the controller between keyboard and joystick HID devices */
typedef enum {
    KEYJOY_STATE_UNINIT,
    KEYJOY_STATE_JOYSTICK,
    KEYJOY_STATE_KEYBOARD
} keyjoy_state_t;

typedef struct {
    uint8_t id;
    unsigned char ascii;
} keyjoy_button_t;

typedef enum {
    STICK_ZERO,
    STICK_NEG,
    STICK_POS
} stick_state_t;

class keyJoystick
{
private:
    keyjoy_state_t _state;
    keyjoy_button_t *_buttons;
    Joystick_ Joystick;

    uint8_t _x_pin;
    stick_state_t _x_state;
    uint8_t _y_pin; // Y axis of module is left/right on the controller
    stick_state_t _y_state;

public:
    keyJoystick(keyjoy_button_t *buttons, uint8_t x_pin, uint8_t y_pin) : _buttons(buttons), _x_pin(x_pin), _y_pin(y_pin) {}
    ~keyJoystick() {}

    void begin();
    void press(uint8_t button);
    void release(uint8_t button);

    void read_stick();

    void update();
};

#endif