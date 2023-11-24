#ifndef KEY_JOYSTICK_H
#define KEY_JOYSTICK_H

#include <Arduino.h>

class keyJoystick
{
private:
    /* data */
    uint8_t _xPin;
    uint8_t _yPin;
public:
    keyJoystick(const uint8_t xPin, const uint8_t yPin ) : _xPin(xPin), _yPin(yPin) {}
    ~keyJoystick() {}

    int read();
};

#endif