#ifndef __AUDIOBITS_H__
#define __AUDIOBITS_H__

#include <Arduino.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// WAV files converted to code by wav2sketch
#include "text_samples/AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "text_samples/AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "text_samples/AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "text_samples/AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "text_samples/AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "text_samples/AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
extern AudioPlayMemory    sound0;
extern AudioPlayMemory    sound1;  // six memory players, so we can play
extern AudioPlayMemory    sound2;  // all six sounds simultaneously
extern AudioPlayMemory    sound3;
extern AudioPlayMemory    sound4;
extern AudioPlayMemory    sound5;
extern AudioMixer4        mix1;    // two 4-channel mixers are needed in
extern AudioMixer4        mix2;    // tandem to combine 6 audio sources
extern AudioOutputI2S     headphones;
extern AudioOutputAnalog  dac;     // play to both I2S audio board and on-chip DAC

// Create Audio connections between the components
//
// Create Audio connections between the components
//
extern AudioConnection c1;
extern AudioConnection c2;
extern AudioConnection c3;
extern AudioConnection c4;
extern AudioConnection c5;   // output of mix1 into 1st input on mix2
extern AudioConnection c6;
extern AudioConnection c7;
extern AudioConnection c8;
extern AudioConnection c9;
extern AudioConnection c10;


// AudioConnection c1(sound0, 0, mix1, 0);
// AudioConnection c2(sound1, 0, mix1, 1);
// AudioConnection c3(sound2, 0, mix1, 2);
// AudioConnection c4(sound3, 0, mix1, 3);
// AudioConnection c5(mix1, 0, mix2, 0);   // output of mix1 into 1st input on mix2
// AudioConnection c6(sound4, 0, mix2, 1);
// AudioConnection c7(sound5, 0, mix2, 2);
// AudioConnection c8(mix2, 0, headphones, 0);
// AudioConnection c9(mix2, 0, headphones, 1);
// AudioConnection c10(mix2, 0, dac, 0);

// Create an object to control the audio shield.
extern AudioControlSGTL5000 audioShield;


void setup_audio_bits();
void sample_switch(int pad_num);

#endif