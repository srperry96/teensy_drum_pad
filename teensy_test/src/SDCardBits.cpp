#include <SDCardBits.h>


//Sample filepath arrays / variables
String pad_sample_paths[16];

String kick_paths[MAX_SAMPLES_PER_TYPE];
String snare_paths[MAX_SAMPLES_PER_TYPE];
String hat_paths[MAX_SAMPLES_PER_TYPE];
String openhat_paths[MAX_SAMPLES_PER_TYPE]; 
String clap_paths[MAX_SAMPLES_PER_TYPE];
uint8_t num_kicks = 0, num_snares = 0, num_hats = 0, num_openhats = 0, num_claps = 0;


//Connect to the SD card
void setup_sd_card(){
    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);

    if(!(SD.begin(SDCARD_CS_PIN))){
        // stop here, but print a message repetitively
        while(1){
            Serial.println("Unable to access the SD card");
            delay(500);
        }
    }
    Serial.println("Setup SD card");

    //Load sample filepaths from SD, and assign them to the Neotrellis pads
    load_sample_filepaths();
}


//List all files - code adapted from teensy SD library here: https://github.com/PaulStoffregen/SD/blob/master/examples/listfiles/listfiles.ino
void printDirectory(File dir, int numTabs){
    while(true){
     
        File entry = dir.openNextFile();

        //if there are no more files, break out of the loop
        if(!entry){
            break;
        }
        for(uint8_t i = 0; i < numTabs; i++){
            Serial.print('\t');
        }
        if(entry.isDirectory()){
            Serial.print(entry.name());
            Serial.println("/");
            printDirectory(entry, numTabs+1);
        }else{
            Serial.println(entry.name());
        }
        entry.close();
    }
}

//List all files on the SD card, printing out via Serial
void list_files_on_sd(){
    Serial.println("Listing files on SD card");
    File root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("End of file structure");
}

//Load filepaths for a single set of samples (kicks, snares etc)
void load_single_set(String paths[20], uint8_t *num_samples, const char *sample_type){
    File root = SD.open(sample_type);
    uint8_t total = 0;

    while(true){
        File entry = root.openNextFile();
        if((!entry) || (total >= MAX_SAMPLES_PER_TYPE)){
            *num_samples = total;
            break;
        }
        if(!entry.isDirectory()){
            paths[total] = entry.name();
            total++;
        }
        entry.close();
    }

    Serial.print("Got filepaths for ");
    Serial.print(sample_type);
    Serial.print(". Total: ");
    Serial.println(*num_samples);
}

//Load in filepaths for all the samples on the SD card
void load_sample_filepaths(){
    load_single_set(kick_paths, &num_kicks, "/kicks");
    load_single_set(snare_paths, &num_snares, "/snares");
    load_single_set(hat_paths, &num_hats, "/hats");
    load_single_set(openhat_paths, &num_openhats, "/openhats");
    load_single_set(clap_paths, &num_claps, "/claps");

    set_default_sample_filepaths();
}


//Setup pad sample filepaths in an organised fashion. (Row 1: Kicks, Row 2: Snares, Row 3: Hats, Row 4: Claps)
void set_default_sample_filepaths(){
    for(uint8_t i = 0; i < 4; i++){
        pad_sample_paths[i + 12] = "/kicks/" + kick_paths[i];
        pad_sample_paths[i + 8] = "/snares/" + snare_paths[i];
        pad_sample_paths[i + 4] = "/hats/" + hat_paths[i];
    }

    pad_sample_paths[0] = "/claps/" + clap_paths[0];
    pad_sample_paths[1] = "/claps/" + clap_paths[1];
    pad_sample_paths[2] = "/openhats/" + openhat_paths[2];
    pad_sample_paths[3] = "/openhats/" + openhat_paths[3];
}