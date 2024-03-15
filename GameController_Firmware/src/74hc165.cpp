#include "74hc165.h"

// #define DEBUG_74HC165
#ifdef DEBUG_74HC165
#define debugf(msg) Serial.println(msg)
#else
#define debugf(msg)
#endif


shift165::shift165(const int dataPin, const int clockPin, const int latchPin, const unsigned int n_devices)
    : _data(dataPin), _clock(clockPin), _latch(latchPin), _n_devices(n_devices)
{
    pinMode(dataPin, INPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
}

void shift165::capture()
{
    digitalWrite(_latch, LOW);
    delayMicroseconds(10); // Short delay
    digitalWrite(_latch, HIGH);

    uint8_t reg[_n_devices] = {0};

    // Read the data from shift registers
    for (int i = _n_devices - 1; i >= 0; i--) {
        reg[i] = (shiftIn(_data, _clock, MSBFIRST));
    }
    _capture = reg[0] + (reg[1] << 8);

    // Old way, stopped working??
    // digitalWrite(_latch, LOW);
    // digitalWrite(_latch, HIGH);

    // for (int i = 0; i < 8*_n_devices; i++) {
    //     int_fast8_t input = (digitalRead(_data) & 1);
    //     if (input == 0) {
    //         _capture &= ~(input << i);
    //     } else {
    //         _capture |= (input << i);
    //     }

    // digitalWrite(_clock, HIGH); // Shift out the next bit
    // digitalWrite(_clock, LOW);
    // }
}

void shift165::init()
{
    pinMode(_data, INPUT);
    pinMode(_clock, OUTPUT);
    pinMode(_latch, OUTPUT);
}

int shift165::read(uint8_t bit)
{
    if (bit >= (_n_devices*8)) {
        return -1;
    }
    debugf(_capture);
    return _capture & (1 << bit);
}