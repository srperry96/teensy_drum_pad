#include <Arduino.h>
#include "NeotrellisBits.h"
#include "AudioBits.h"
#include "SDCardBits.h"

#define MILLIS_PER_MIN 60000

unsigned long start_millis, current_millis;
const uint8_t neotrellis_period = 20; //neotrellis has a resolution of around 60hz

unsigned long start_tempo_millis, current_tempo_millis;

void setup() {
    Serial.begin(9600);
  
    //Setup hardware
    setup_neotrellis();
    setup_audio_bits();
    setup_sd_card();

    //Load sample filepaths from SD, and assign them to the Neotrellis pads
    load_sample_filepaths();

    //get start millis for timing, instead of just using delays
    start_millis = millis();
    start_tempo_millis = millis();
}

uint8_t btn_count = 0;

uint8_t metronome_tempo = 100;
uint8_t metronome_period = MILLIS_PER_MIN / metronome_tempo;
bool metronome_on = false;


void loop() {
    current_millis = millis();

    current_tempo_millis = current_millis;
  
    if(metronome_on){
      if(current_tempo_millis - start_tempo_millis > metronome_period){
        play_metronome();
        start_tempo_millis = current_tempo_millis;
      }
    }



    //Read neotrellis at the specified rate
    if(current_millis - start_millis > neotrellis_period){
        trellis.read();
    
        //check pad states, play corresponding sample if needed
        for(btn_count = 0; btn_count < 16; btn_count++){
          if(button_states[btn_count] == 1){
            play_pad_sample(btn_count);

            //reset the button state so we dont double play the sample
            button_states[btn_count] = 0;
          }
        }
        start_millis = current_millis;
    }
}