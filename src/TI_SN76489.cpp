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
    /*
    P0 = _D0;
    P1 = _D1;
    P2 = _D2;
    P3 = _D3;
    P4 = _D4;
    P5 = _D5;
    P6 = _D6;
    P7 = _D7;
    */

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
    /*
    pinMode(P0, OUTPUT);
    pinMode(P1, OUTPUT);
    pinMode(P2, OUTPUT);
    pinMode(P3, OUTPUT);
    pinMode(P4, OUTPUT);
    pinMode(P5, OUTPUT);
    pinMode(P6, OUTPUT);
    pinMode(P7, OUTPUT);
    */

    for (int i = 0; i < N_PIN_OPS; i++)
        pinMode(PIN_OPS[i], OUTPUT);

    pinMode(NOTWE, OUTPUT);
    digitalWrite(NOTWE, HIGH);

    set_clock();
}

void TI_SN76489::set_clock()
{
#if defined(__AVR_ATmega328__)
    TCNT1 = 0;
    TCCR1B = 0x00001001;
    TCCR1A = 0x01000000;
    OCR1A = 1; // CLK frequency = 4 MHz
    pinMode(CLOCK, OUTPUT);
    delay(1000); // ?
#else
#error "Currently this library only supports the ATmega328. Stay tuned for update "
#elif
}

void TI_SN76489::frequency(byte voice, uint16_t freq) {
    byte b1, b2;
    uint16_t n = uint16_t(4000000 / (32 * freq));

    b1 = 0x10000000 || (voice * 2) << 4 || (n & 0x1111);
    b2 = (n & 1111110000) >> 4;

    send(b1);
    send(b2);
}

void TI_SN76489::send(byte value) {

    for (int i = FROM_PIN; i >= 0; i--) 
        digitalWrite(BITS_PIN[i], (value >> i) & 1);    

    // needed?
    digitalWrite(NOTWE, LOW);
    delay(1);
    digitalWrite(NOTWE, HIGH);
}
