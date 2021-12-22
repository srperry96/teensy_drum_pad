#ifndef __NEOTRELLISBITS_H__
#define __NEOTRELLISBITS_H__

#include <Arduino.h>
#include "Adafruit_NeoTrellis.h"

#define BUTTON_HOLD_LIMIT 1500

TrellisCallback pad_callback(keyEvent evt);

class Neotrellis{
    public:
        Neotrellis();
        uint32_t Wheel(byte WheelPos);
        
        Adafruit_NeoTrellis trellis;
        const uint8_t neotrellis_period = 20; //neotrellis has a resolution of around 60hz
        
        uint8_t button_presses[16] = {0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,0};

        elapsedMillis button_hold_counter = 0;
        int8_t held_button_id = -1;

};

#endif