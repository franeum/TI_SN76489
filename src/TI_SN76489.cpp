#include "TI_SN76489.h"

#define N_PIN_OPS 8
#define FROM_PIN 7

TI_SN76489::TI_SN76489(uint8_t _D0,
                       uint8_t _D1,
                       uint8_t _D2,
                       uint8_t _D3,
                       uint8_t _D4,
                       uint8_t _D5,
                       uint8_t _D6,
                       uint8_t _D7,
                       uint8_t _NOT_WE,
                       uint8_t _CLOCK_PIN,
                       ClockFrequency _FREQUENCY)
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
    CLOCK = _CLOCK_PIN;
    FREQUENCY = _FREQUENCY;

    switch ((byte)FREQUENCY)
    {
    case _4MHz:
        clock_frequency = 4000000;
        break;
    case _2MHz:
        clock_frequency = 2000000;
        break;
    case _1MHz:
        clock_frequency = 1000000;
        break;
    default:
        clock_frequency = 4000000;
    }
}

void TI_SN76489::begin()
{
    set_clock();

    for (int i = 0; i < N_PIN_OPS; i++)
        pinMode(PIN_OPS[i], OUTPUT);

    pinMode(NOTWE, OUTPUT);
    digitalWrite(NOTWE, HIGH);
}

void TI_SN76489::set_clock()
{

#if defined(__AVR_ATmega328P__)
    DDRB = 1 << DDB1;
    TCNT1 = 0;
    TCCR1B = 0b00001001;
    TCCR1A = 0b01000000;
    OCR1A = FREQUENCY; // CLK frequency = 1: 4MHz, 3: 2MHz, 7: 1MHz
    // OCR1A = 1;

#elif defined(__AVR_ATmega32U4__)
    DDRB = 1 << DDB5; // set PB5 to output
    TCCR1B = 0b00001001;
    TCCR1A = 0b01000000;
    TIMSK1 = 0;
    TCNT1 = 0;
    OCR1A = FREQUENCY; // CLK frequency = 1: 4MHz, 3: 2MHz, 7: 1MHz
#else
#error "Currently this library only supports the ATmega328P or ATmega32u4 Stay tuned for update "
#endif

    delay(1000);
}

int TI_SN76489::frequency(uint8_t voice, float freq)
{
    byte b1;
    byte b2 = NULL;

    uint16_t n = clock_frequency / (32.0 * freq);

    Serial.print("clock:\t");
    Serial.println(clock_frequency);

    Serial.print("OCR1A:\t");
    Serial.println(FREQUENCY);

    Serial.print("factor:\t");
    Serial.println(n);

    if (voice == 3)
    {
        noise(freq);
        return 0;
    }

    b1 = 0b10000000 | get_reg(voice) | (n & 0b00001111);
    b2 = (n >> 4) & 0b00111111;

    Serial.print("b1:\t");
    Serial.println(b1);
    Serial.print("b2:\t");
    Serial.println(b2);

    send(b1);
    send(b2);

    return 0;
}

void TI_SN76489::noise(float control)
{
    /*  000 (0): periodic high
        001 (1): periodic medium
        010 (2): periodic low

        100 (4): white high
        101 (5): white medium
        110 (6): white low
    */
    uint8_t ctrl = min((uint8_t)control, 6);
    byte b1 = 0;

    b1 = 0b11100000 | ctrl;

    send(b1);
}

void TI_SN76489::attenuation(uint8_t voice, uint8_t atten)
{
    uint8_t b1;

    // b1 = 0b10000000 | get_reg(voice) + 16 | 0b1111 & atten;
    // the same:
    b1 = 0b10000000 | get_reg(voice) | 0b10000 | 0b1111 & atten;

    send(b1);
}

void TI_SN76489::send(byte value)
{

    /*
        for (int i = FROM_PIN; i >= 0; i--)
            digitalWrite(PIN_OPS[i], (value >> i) & 1);
            */

    /*
        digitalWrite(2, value & 128);
        digitalWrite(3, value & 64);
        digitalWrite(4, value & 32);
        digitalWrite(5, value & 16);
        digitalWrite(6, value & 8);
        digitalWrite(7, value & 4);
        digitalWrite(8, value & 2);
        digitalWrite(10, value & 1);


       digitalWrite(2, value & 128);
        digitalWrite(3, value & 64);
        digitalWrite(4, value & 32);
        digitalWrite(5, value & 16);
        digitalWrite(6, value & 8);
        digitalWrite(7, value & 4);
        digitalWrite(8, value & 2);
        */

    digitalWrite(10, (value & 1) ? HIGH : LOW);
    digitalWrite(8, (value & 2) ? HIGH : LOW);
    digitalWrite(7, (value & 4) ? HIGH : LOW);
    digitalWrite(6, (value & 8) ? HIGH : LOW);
    digitalWrite(5, (value & 16) ? HIGH : LOW);
    digitalWrite(4, (value & 32) ? HIGH : LOW);
    digitalWrite(3, (value & 64) ? HIGH : LOW);
    digitalWrite(2, (value & 128) ? HIGH : LOW);

    // needed?
    digitalWrite(NOTWE, LOW);
    delay(1);
    digitalWrite(NOTWE, HIGH);
}
