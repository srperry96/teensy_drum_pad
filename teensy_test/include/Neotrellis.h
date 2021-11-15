#ifndef __NEOTRELLIS_H__
#define __NEOTRELLIS_H__

#include <Arduino.h>
#include "Adafruit_NeoTrellis.h"

uint32_t Wheel(byte WheelPos);
TrellisCallback blink(keyEvent evt);
bool setup_neotrellis();

extern Adafruit_NeoTrellis trellis;
extern uint8_t button_states[16];

#endif