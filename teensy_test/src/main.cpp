#include <Arduino.h>
#include "NeotrellisBits.h"
#include "AudioBits.h"
#include "SDCardBits.h"

// #include <SD.h>
// #include <SPI.h>


// File root;

// //List all files - code from teensy SD library here: https://github.com/PaulStoffregen/SD/blob/master/examples/listfiles/listfiles.ino
// void printDirectory(File dir, int numTabs) {
//    while(true) {
     
//      File entry =  dir.openNextFile();

//      //if there are no more files, break out of the loop
//      if (! entry) {
//        break;
//      }
//      for (uint8_t i=0; i<numTabs; i++) {
//        Serial.print('\t');
//      }
//      Serial.print(entry.name());
//      if (entry.isDirectory()) {
//        Serial.println("/");
//        printDirectory(entry, numTabs+1);
//      } else {
//        // files have sizes, directories do not
//        Serial.print("\t\t");
//        Serial.println(entry.size(), DEC);
//      }
//      entry.close();
//    }
// }


unsigned long start_millis, current_millis;
const uint8_t neotrellis_period = 20; //neotrellis has a resolution of around 60hz



void setup() {
  Serial.begin(9600);

  //Setup neotrellis
  if(!setup_neotrellis()){
    Serial.println("Couldnt set up neotrellis");
    while(1);
  }

  //Setup audio (includes SD card)
  setup_audio_bits();

  setup_sd_card();

  load_sample_filepaths();

  start_millis = millis();
}

uint8_t count;



void loop() {
  
  //millis for timing, instead of delays which might interfere with eachother
  current_millis = millis();
  
  //Read neotrellis if enough time has passed since last read
  if(current_millis - start_millis > neotrellis_period){
    trellis.read();
    start_millis = current_millis;
  
    //
    for(count = 0; count < 16; count++){
      if(button_states[count] == 1){
        sample_switch(count);
        //reset the button state so we dont double play the sample
        button_states[count] = 0;
      }
    }
  }

}