#ifndef __NEOTRELLISBITS_H__
#define __NEOTRELLISBITS_H__

#include <Arduino.h>
#include "Adafruit_NeoTrellis.h"

uint32_t Wheel(byte WheelPos);
TrellisCallback blink(keyEvent evt);
void setup_neotrellis();

extern Adafruit_NeoTrellis trellis;
extern bool button_update;
extern uint8_t button_states[16];

#endif