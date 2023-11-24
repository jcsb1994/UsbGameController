#include "keyJoystick.h"

int keyJoystick::read()
{
    int x = analogRead(_xPin);
    int y = analogRead(_yPin);
}