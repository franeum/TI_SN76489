#ifndef TI_SN76489_H
#define TI_SN76489_H

#include <Arduino.h>

class TI_SN76489
{
public:
    TI_SN76489(){};
    TI_SN76489(int pin, byte channel, byte controller, String identifier = "POT")
    {
        begin(pin, channel, controller, identifier);
    };

    static void set_debug(byte value);
    void begin(int pin, byte channel, byte controller, String identifier = "POT");
    void update();
    inline void set_reverse() { reverse = true; };
    inline void set_forward() { reverse = false; };

private:
    static int _debug;
    bool reverse = false;
    int pin;
    ResponsiveAnalogRead *pot;
    int previous_value;
    byte _channel;
    byte _controller;
    void send(int value);
    int parseValue(int v);
    String _id;
};

#endif
