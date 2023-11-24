#ifndef _74HC165_H
#define _74HC165_H
#include <Arduino.h>

class shift165
{
public:
    shift165(const int dataPin, const int clockPin, const int latchPin, const unsigned int n_devices = 1);
    ~shift165() {}

    void capture();
    int read(unsigned int bit);
private:
    const int _data;   /* Q7 (PIN 9) */
    const int _clock;  /* CP (PIN ) */
    const int _latch;  /* PL (PIN ) */
    const unsigned int _n_devices;
    uint32_t _capture = 0;
};


#endif