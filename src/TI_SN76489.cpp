#include "TI_SN76489.h"

#define N_PIN_OPS 8
#define FROM_PIN 7

TI_SN76489::TI_SN76489(byte _CLOCK,
                       byte _D0,
                       byte _D1,
                       byte _D2,
                       byte _D3,
                       byte _D4,
                       byte _D5,
                       byte _D6,
                       byte _D7,
                       byte _NOT_WE)
{
    PIN_OPS[0] = _D0;
    PIN_OPS[1] = _D1;
    PIN_OPS[2] = _D2;
    PIN_OPS[3] = _D3;
    PIN_OPS[4] = _D4;
    PIN_OPS[5] = _D5;
    PIN_OPS[6] = _D6;
    PIN_OPS[7] = _D7;

    NOTWE = _NOT_WE;
    CLOCK = _CLOCK;
}

void TI_SN76489::begin()
{
    for (int i = 0; i < N_PIN_OPS; i++)
        pinMode(PIN_OPS[i], OUTPUT);

    pinMode(NOTWE, OUTPUT);
    digitalWrite(NOTWE, HIGH);

    set_clock();
}

void TI_SN76489::set_clock()
{
#if defined(__AVR_ATmega328__)
    DDRB = 1 << DDB1;
    TCNT1 = 0;
    TCCR1B = 0x00001001;
    TCCR1A = 0x01000000;
    OCR1A = 1; // CLK frequency = 1: 4MHz, 3: 2MHz, 7: 1MHz

#elif defined(__AVR_ATmega32U4__)
    DDRB = 1 << DDB5; // set PB5 to output
    TCCR1B = 0x00001001;
    TCCR1A = 0x01000000;
    TIMSK1 = 0;
    TCNT1 = 0;
    OCR1A = 1; // CLK frequency = 1: 4MHz, 3: 2MHz, 7: 1MHz
#else
#error "Currently this library only supports the ATmega328 or ATmega32u4 Stay tuned for update "
#endif

    delay(1000);
}

void TI_SN76489::frequency(byte voice, uint16_t freq)
{
    byte b1, b2;
    uint16_t n = uint16_t(4000000 / (32 * freq));

    b1 = 0x10000000 | get_reg(voice) | (n & 0x1111);
    b2 = (n & 1111110000) >> 4;

    send(b1);
    send(b2);
}

void TI_SN76489::attenuation(byte voice, byte atten)
{
    byte b1;

    b1 = 0x10000000 | get_reg(voice) + 16 | 0x1111 & atten;

    send(b1);
}

void TI_SN76489::send(byte value)
{

    for (int i = FROM_PIN; i >= 0; i--)
        digitalWrite(PIN_OPS[i], (value >> i) & 1);

    // needed?
    digitalWrite(NOTWE, LOW);
    delay(1);
    digitalWrite(NOTWE, HIGH);
}
