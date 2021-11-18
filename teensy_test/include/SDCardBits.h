#ifndef __SDCARDBITS_H__
#define __SDCARDBITS_H__

#include <SD.h>
#include <SPI.h>


//SD card pin definitions for use with teensy audio board
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

//for now, assume we have max 20 of each type of sample on the sd card
#define MAX_SAMPLES_PER_TYPE 20

//Functions for printing SD card file structure out via Serial
void printDirectory(File dir, int numTabs);
void list_files_on_sd();

//Connect to the SD card
void setup_sd_card();

//Load in filepaths for all the samples
void load_sample_filepaths();


//Arrays to store sample filepaths
extern String kick_paths[MAX_SAMPLES_PER_TYPE];
extern String snare_paths[MAX_SAMPLES_PER_TYPE];
extern String hat_paths[MAX_SAMPLES_PER_TYPE];
extern String openhat_paths[MAX_SAMPLES_PER_TYPE];
extern String clap_paths[MAX_SAMPLES_PER_TYPE];

//numbers of each type of sample
extern uint8_t num_kicks, num_snares, num_hats, num_openhats, num_claps;


#endif