#include <SDCardBits.h>

//Setting up sample filepath arrays / variables
String kick_paths[MAX_SAMPLES_PER_TYPE], snare_paths[MAX_SAMPLES_PER_TYPE], hat_paths[MAX_SAMPLES_PER_TYPE], openhat_paths[MAX_SAMPLES_PER_TYPE], clap_paths[MAX_SAMPLES_PER_TYPE];
uint8_t num_kicks = 0, num_snares = 0, num_hats = 0, num_openhats = 0, num_claps = 0;


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
        //  Serial.print(entry.name());
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

void list_files_on_sd(){
    Serial.println("Listing files on SD card");
    File root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("End of file structure");
}


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


void load_sample_filepaths(){

    //kicks
    load_single_set(kick_paths, &num_kicks, "/kicks");

    //snares
    load_single_set(snare_paths, &num_snares, "/snares");

    //hats
    load_single_set(hat_paths, &num_hats, "/hats");

    //openhats
    load_single_set(openhat_paths, &num_openhats, "/openhats");

    //claps
    load_single_set(clap_paths, &num_claps, "/claps");
}
