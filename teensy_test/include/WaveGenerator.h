#ifndef __WAVEGENERATOR_H__
#define __WAVEGENERATOR_H__

#include <Audio.h>
#include "AudioBits.h"

//these are instantiated in AudioBits.h as they need to be there for the patches to work properly
extern AudioSynthWaveform waveform1;
extern AudioFilterStateVariable filter1;


void start_sin();

void set_freq(int freq);

void stop_sin();

void set_filter_freq(int freq);

extern bool wave_playing;

#endif