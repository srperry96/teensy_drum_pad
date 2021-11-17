#ifndef __AUDIOBITS_H__
#define __AUDIOBITS_H__

#include <Arduino.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>


//SD card pin definitions for use with teensy audio board
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14


void setup_audio_bits();
void sample_switch(int pad_num);
void playFile(const char *filename);


#endif