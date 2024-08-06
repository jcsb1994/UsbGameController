#include "keyJoystick.h"


void keyJoystick::begin()
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

void keyJoystick::read_stick()
{
    int x_read = analogRead(_x_pin);
    int y_read = analogRead(_y_pin);

    if (_state == KEYJOY_STATE_KEYBOARD) {

        if(y_read > 700) {
            if (_y_state != STICK_POS) {
                Keyboard.press('A');
                _y_state = STICK_POS;
            }
        } else if (_y_state == STICK_POS) {
            Keyboard.release('A');
            _y_state = STICK_ZERO;
        }
        
        if(y_read < 400) {
            if (_y_state != STICK_NEG) {
                Keyboard.press('D');
                _y_state = STICK_NEG;
            }
        } else if (_y_state == STICK_NEG) {
            Keyboard.release('D');
            _y_state = STICK_ZERO;
        }

        if(x_read > 700) {
            if (_y_state != STICK_POS) {
                Keyboard.press('W');
                _x_state = STICK_POS;
            }
        } else  if (_x_state == STICK_POS) {
            Keyboard.release('W');
            _x_state = STICK_ZERO;
        }
        
        if(x_read < 400) {
            if (_y_state != STICK_NEG) {
                Keyboard.press('S');
                _x_state = STICK_NEG;
            }
        } else  if (_x_state == STICK_NEG) {
            Keyboard.release('S');
            _x_state = STICK_ZERO;
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