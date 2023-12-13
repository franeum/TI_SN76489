#ifndef TI_SN76489_H
#define TI_SN76489_H

#include <Arduino.h>

typedef enum
{
    _4MHz = 1,
    _2MHz = 3,
    _1MHz = 7
} ClockFrequency;

class TI_SN76489
{
public:
    TI_SN76489(uint8_t _D0,
               uint8_t _D1,
               uint8_t _D2,
               uint8_t _D3,
               uint8_t _D4,
               uint8_t _D5,
               uint8_t _D6,
               uint8_t _D7,
               uint8_t _NOT_WE,
               uint8_t _CLOCK_PIN,
               ClockFrequency _FREQUENCY = _4MHz);

    void begin();
    int frequency(uint8_t voice, float freq);
    void attenuation(uint8_t voice, uint8_t atten);

private:
    uint8_t PIN_OPS[8];
    uint8_t NOTWE;
    uint8_t CLOCK;
    float clock_frequency;
    ClockFrequency FREQUENCY;
    inline uint8_t get_reg(uint8_t voice) { return (voice * 2) << 4; }
    void set_clock();
    void send(byte value);
    void noise(float control);
};

#endif
