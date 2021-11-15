#include <Arduino.h>
#include "Neotrellis.h"
#include "AudioBits.h"

void setup() {
  Serial.begin(9600);
  if(!setup_neotrellis()){
    Serial.println("Couldnt set up neotrellis");
    while(1);
  }

  setup_audio_bits();

  Serial.println("Audio bits setup (hopefully)");


}

uint8_t count;

void loop() {
  trellis.read();  // interrupt management does all the work! :)
  
  for(count = 0; count < 16; count++){
    if(button_states[count] == 1){
      sample_switch(count);
      //reset the button state so we dont double play the sample
      button_states[count] = 0;
    }
  }

  delay(20); //the trellis has a resolution of around 60hz
}