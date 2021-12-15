#include <Arduino.h>
#include "SDCardBits.h"
#include "AudioBits.h"
#include "SharedBits.h"

#define POT_VOL A1
#define POT2 A11
#define POT3 A3
#define POT4 A10

elapsedMillis neotrellis_millis, tempo_millis, waveform_millis, pot_check_millis;


uint8_t btn_count = 0;
uint16_t pot1, pot2, pot3, pot4;
uint8_t pot_check_period = 10;

Sound sound;

void setup() {
    Serial.begin(9600);

    //Setup SD card. IMPORTANT: This has to be done here, not in another file / in the MicroSD constructor. Not entirely sure why but it works here.
    sd_card.setup_sd_card();

    //get start millis for timing, instead of just using delays
    neotrellis_millis = 0;
    tempo_millis = 0;
    waveform_millis = 0;
    pot_check_millis = 0;

}


void loop() {

  if(tempo_millis > metronome.period){
    if(metronome.on){
      sound.play_metronome();
    }
    tempo_millis = 0;
  }


  //Read neotrellis at the specified rate
  if(neotrellis_millis > neo.neotrellis_period){
    neo.trellis.read();
  
    //check pad states, play corresponding sample if needed
    for(btn_count = 0; btn_count < 16; btn_count++){
      if(neo.button_states[btn_count] == 1){
        if(pot4 < 500){ //THIS WILL BE CHANGED TO A PROPER COMPARISON AT SOME POINT. FOR NOW, TURNING POT4 SWITCHES MODES
          sound.play_pad_sample(btn_count);

        //else play corresponding note    
        }else{
          if(!wavegen.wave_playing){
            wavegen.start_osc1();
            wavegen.start_osc2();
            wavegen.wave_playing = true;
          }
          wavegen.play_pad_note(btn_count);
        }

        //reset the button state so we dont double play the sample
        neo.button_states[btn_count] = 0;
      }
    }
    neotrellis_millis = 0;
  }


    if(pot_check_millis > pot_check_period){

      pot1 = analogRead(POT_VOL);
      pot2 = analogRead(POT2);
      pot3 = analogRead(POT3);
      pot4 = analogRead(POT4);


      sound.update_volume(pot1);

      if(wavegen.wave_playing){
        wavegen.set_filter_freq(pot2);
        wavegen.set_osc2_detune(pot3);
      
        if(pot4 < 500){
          wavegen.stop_osc1();
          wavegen.stop_osc2();
          wavegen.wave_playing = false;
        }
      }

      pot_check_millis = 0;
  }
}