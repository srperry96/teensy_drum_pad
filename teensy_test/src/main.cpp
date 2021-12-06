#include <Arduino.h>
#include "NeotrellisBits.h"
#include "AudioBits.h"
#include "SDCardBits.h"
#include "TFTScreenBits.h"

unsigned long neotrellis_millis, current_millis;
unsigned long tempo_millis, waveform_millis, pot_check_millis;



void setup() {
    Serial.begin(9600);

    //Setup hardware
    setup_neotrellis();
    setup_audio_bits();
    setup_sd_card();

    setup_screen();

    //Load sample filepaths from SD, and assign them to the Neotrellis pads
    load_sample_filepaths();

    //get start millis for timing, instead of just using delays
    neotrellis_millis = millis();
    tempo_millis = neotrellis_millis;

    waveform_millis = neotrellis_millis;
    pot_check_millis = neotrellis_millis;

}

uint8_t btn_count = 0;

uint16_t pot1, pot2, pot3, pot4;
uint8_t pot_check_period = 10;


void loop() {
  current_millis = millis();

  if(metronome_on){
    if(current_millis - tempo_millis > metronome_period){
      play_metronome();
      tempo_millis = current_millis;
    }
  }



  //Read neotrellis at the specified rate
  if(current_millis - neotrellis_millis > neotrellis_period){
    trellis.read();
  
    //check pad states, play corresponding sample if needed
    for(btn_count = 0; btn_count < 16; btn_count++){
      if(button_states[btn_count] == 1){
        play_pad_sample(btn_count);

        //reset the button state so we dont double play the sample
        button_states[btn_count] = 0;
      }
    }
    neotrellis_millis = current_millis;
  }



  if(current_millis - pot_check_millis > pot_check_period){

    pot1 = analogRead(A1);
    pot2 = analogRead(A11);
    pot3 = analogRead(A3);
    pot4 = analogRead(A10);




    //change volume if knob has moved enough (+- 2 accounts for noisy analog readings)
    if(!((current_volume > pot1 - 2) && (current_volume < pot1 + 2))){
      set_volume(pot1);
    }

    if(pot4 > 500){
      if(!wave_playing){
        start_sin();
        wave_playing = true;
      }
      set_filter_freq(pot2);
      set_freq(pot3);
    }else{
      stop_sin();
      wave_playing = false;
    }

    pot_check_millis = current_millis;
  }
}