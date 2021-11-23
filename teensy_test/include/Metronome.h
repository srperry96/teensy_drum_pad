#ifndef __METRONOME_H__
#define __METRONOME_H__

// Audio data converted from audio file by wav2sketch_js
#include <Arduino.h>

#define MILLIS_PER_MIN 60000

extern const unsigned int AudioSampleMetronome_click[449];


extern uint8_t metronome_tempo;
extern uint8_t metronome_period;
extern bool metronome_on;

#endif