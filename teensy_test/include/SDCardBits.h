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


class MicroSD{
    public:
        MicroSD();

        //Functions for printing SD card file structure out via Serial
        void printDirectory(File dir, int numTabs);
        void list_files_on_sd();

        //Connect to the SD card
        void setup_sd_card();

        //Load in filepaths for all the samples
        void load_sample_filepaths();

        void set_default_sample_filepaths();
        void load_single_set(String paths[20], uint8_t *num_samples, const char *sample_type);


        //Arrays to store sample filepaths
        String kick_paths[MAX_SAMPLES_PER_TYPE];
        String snare_paths[MAX_SAMPLES_PER_TYPE];
        String hat_paths[MAX_SAMPLES_PER_TYPE];
        String openhat_paths[MAX_SAMPLES_PER_TYPE];
        String clap_paths[MAX_SAMPLES_PER_TYPE];

        String pad_sample_paths[16];

        //numbers of each type of sample
        uint8_t num_kicks = 0, num_snares = 0, num_hats = 0, num_openhats = 0, num_claps = 0;
};

#endif