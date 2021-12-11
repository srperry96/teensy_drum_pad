#ifndef __NEOTRELLISBITS_H__
#define __NEOTRELLISBITS_H__

#include <Arduino.h>
#include "Adafruit_NeoTrellis.h"

TrellisCallback blink(keyEvent evt);

class Neotrellis{
    public:
        Neotrellis();
        uint32_t Wheel(byte WheelPos);
        
        Adafruit_NeoTrellis trellis;
        const uint8_t neotrellis_period = 20; //neotrellis has a resolution of around 60hz
        
        uint8_t button_states[16] = {0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,0};

};

#endif