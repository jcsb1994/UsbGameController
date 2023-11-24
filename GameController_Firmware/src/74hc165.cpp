#include "74hc165.h"

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
    digitalWrite(_latch, HIGH);

    for (int i = 0; i < 8*_n_devices; i++) {
        int_fast8_t input = (digitalRead(_data) & 1);
        if (input == 0) {
            _capture &= ~(input << i);
        } else {
            _capture |= (input << i);
        }

    digitalWrite(_clock, HIGH); // Shift out the next bit
    digitalWrite(_clock, LOW);
    }
}

int shift165::read(unsigned int bit)
{
    if (bit >= (_n_devices*8)) {
        return -1;
    }
    return _capture & (1 << bit);
}