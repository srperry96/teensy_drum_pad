#ifndef __AUDIOBITS_H__
#define __AUDIOBITS_H__

#include <Audio.h>
#include "SDCardBits.h"
#include "Metronome.h"

#include "TFTScreenBits.h"

#include "WaveGenerator.h"


void setup_audio_bits();

void play_pad_sample(int pad_num);

void play_file(const char *filename);

void update_pad_sample_filepaths();

void play_metronome();

void set_volume(int volume);

extern uint16_t current_volume;

#endif