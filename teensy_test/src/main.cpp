#include <Arduino.h>
#include "SDCardBits.h"
#include "SharedBits.h"

unsigned long neotrellis_millis, current_millis;
unsigned long tempo_millis, waveform_millis, pot_check_millis;

uint8_t btn_count = 0;
uint16_t pot1, pot2, pot3, pot4;
uint8_t pot_check_period = 10;


void setup() {
    Serial.begin(9600);

    //Setup hardware
    setup_sd_card();

    //get start millis for timing, instead of just using delays
    neotrellis_millis = millis();
    tempo_millis = neotrellis_millis;

    waveform_millis = neotrellis_millis;
    pot_check_millis = neotrellis_millis;

}


void loop() {
  current_millis = millis();

  if(metronome_on){
    if(current_millis - tempo_millis > metronome_period){
      sound.play_metronome();
      tempo_millis = current_millis;
    }
  }



  //Read neotrellis at the specified rate
  if(current_millis - neotrellis_millis > neo.neotrellis_period){
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
    neotrellis_millis = current_millis;
  }



  if(current_millis - pot_check_millis > pot_check_period){

    pot1 = analogRead(A1);
    pot2 = analogRead(A11);
    pot3 = analogRead(A3);
    pot4 = analogRead(A10);




    //change volume if knob has moved enough (+- 2 accounts for noisy analog readings)
    if(!((sound.current_volume > pot1 - 2) && (sound.current_volume < pot1 + 2))){
      sound.set_volume(pot1);
    }

    if(wavegen.wave_playing){
      wavegen.set_filter_freq(pot2);
      wavegen.set_osc2_detune(pot3);
    
      if(pot4 < 500){
        wavegen.stop_osc1();
        wavegen.stop_osc2();
        wavegen.wave_playing = false;
      }
    }

    pot_check_millis = current_millis;
  }
}