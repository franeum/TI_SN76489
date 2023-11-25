#ifndef TI_SN76489_H
#define TI_SN76489_H

#include <Arduino.h>

class TI_SN76489
{
public:
    TI_SN76489(byte CLOCK,
               byte _D0,
               byte _D1,
               byte _D2,
               byte _D3,
               byte _D4,
               byte _D5,
               byte _D6,
               byte _D7,
               byte _NOT_WE);

    void begin();
    void frequency(byte voice, uint16_t freq);
    void attenuation(byte voice, byte atten);

private:
    // byte P0, P1, P2, P3, P4, P5, P6, P7;
    byte PIN_OPS[8];
    byte NOTWE;
    byte CLOCK;
    inline byte get_reg(byte voice) { return (voice * 2) << 4; }
    void set_clock();
    void send(byte value);
};

#endif
