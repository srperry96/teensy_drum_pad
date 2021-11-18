#ifndef __AUDIOBITS_H__
#define __AUDIOBITS_H__

#include <Audio.h>

void setup_audio_bits();

void sample_switch(int pad_num);

void playFile(const char *filename);


extern String filepath_array[16];

#endif