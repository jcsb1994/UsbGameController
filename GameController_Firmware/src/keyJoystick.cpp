#include "keyJoystick.h"


int keyJoystick::begin()
{
    switch (_state) {
        
    case KEYJOY_STATE_JOYSTICK:
        _state = KEYJOY_STATE_KEYBOARD;
        Joystick.end();
        delay(100);
        Keyboard.begin();
        break;
    case KEYJOY_STATE_KEYBOARD:
        Keyboard.releaseAll();
        Keyboard.end();
    case KEYJOY_STATE_UNINIT:
        _state = KEYJOY_STATE_JOYSTICK;
        delay(100);
        Joystick.begin(false);
        break;
    
    default:
        break;
    }
    // debugf("Begin as " + String(_state));
}
void keyJoystick::press(uint8_t button)
{
    if (_state == KEYJOY_STATE_KEYBOARD) {
        Keyboard.press(_buttons[button].ascii);
    } else {
        Joystick.setButton(_buttons[button].id, 1);
    }
}
void keyJoystick::release(uint8_t button)
{
    if (_state == KEYJOY_STATE_KEYBOARD) {
        Keyboard.release(_buttons[button].ascii);
    } else {
        Joystick.setButton(_buttons[button].id, 0);
    }
}

void keyJoystick::read_stick(uint8_t x_pin, uint8_t y_pin)
{
    int x_read = analogRead(x_pin);
    int y_read = analogRead(y_pin);

    if (_state == KEYJOY_STATE_KEYBOARD) {

        if(y_read > 700) {
            Keyboard.press('A');
        } else {
            Keyboard.release('A');
        }
        
        if(y_read < 400) {
            Keyboard.press('D');
        } else {
            Keyboard.release('D');
        }

        if(x_read > 700) {
            Keyboard.press('W');
        } else {
            Keyboard.release('W');
        }
        
        if(x_read < 400) {
            Keyboard.press('S');
        } else {
            Keyboard.release('S');
        } 

    } else {
        Joystick.setXAxis(x_read);
        Joystick.setYAxis(y_read);
    }
}

void keyJoystick::update()
{
    if (_state == KEYJOY_STATE_JOYSTICK) {
        Joystick.sendState(); // Update USB state
    }
}