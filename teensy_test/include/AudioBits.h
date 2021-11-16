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
#include "pjrc_example_samples/AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "pjrc_example_samples/AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "pjrc_example_samples/AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "pjrc_example_samples/AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
// #include "pjrc_example_samples/AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "pjrc_example_samples/AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/

#include "pjrc_example_samples/AudioSampleAshyknee_kick.h"


//SD card pin definitions for use with teensy audio board
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14



// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
extern AudioPlayMemory    sound0;
extern AudioPlayMemory    sound1;  // six memory players, so we can play
extern AudioPlayMemory    sound2;  // all six sounds simultaneously
extern AudioPlayMemory    sound3;
extern AudioPlayMemory    sound4;
// extern AudioPlayMemory    sound5;
extern AudioMixer4        mix1;    // two 4-channel mixers are needed in
extern AudioMixer4        mix2;    // tandem to combine 6 audio sources
// extern AudioMixer4 mix3;
// extern AudioMixer4 mix4;


extern AudioOutputI2S     headphones;
// extern AudioOutputAnalog  dac;     // play to both I2S audio board and on-chip DAC

// Create Audio connections between the components
//
// Create Audio connections between the components
//
// extern AudioConnection c1;
// extern AudioConnection c2;
// extern AudioConnection c3;
// extern AudioConnection c4;
// extern AudioConnection c5;   // output of mix1 into 1st input on mix2
// extern AudioConnection c6;
// extern AudioConnection c7;
// extern AudioConnection c8;
// extern AudioConnection c9;
// extern AudioConnection c10;

//attempting to do sd card stuff
extern AudioPlaySdWav playWav1;
// extern AudioConnection patch1;
// extern AudioConnection patch2;
// extern AudioConnection patch3;



// Create an object to control the audio shield.
extern AudioControlSGTL5000 audioShield;


void setup_audio_bits();
void sample_switch(int pad_num);
void playFile(const char *filename);


#endif