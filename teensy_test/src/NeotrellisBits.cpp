#include <NeotrellisBits.h>
#include "SharedBits.h"


Neotrellis neo;

//Setup the neotrellis keypad
Neotrellis::Neotrellis(){  

  if(!trellis.begin()){
    Serial.println("Couldnt set up neotrellis");
    while(1);
  }

  //activate all keys and set callbacks
  for(int i=0; i<NEO_TRELLIS_NUM_KEYS; i++){
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_RISING);
    trellis.activateKey(i, SEESAW_KEYPAD_EDGE_FALLING);
    trellis.registerCallback(i, pad_callback);
  }

  Serial.println("Setup Neotrellis");

  //do a little animation to show we're on
  //turn pixels on one by one
  for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, Wheel(map(i, 0, trellis.pixels.numPixels(), 0, 255)));
    trellis.pixels.show();
    delay(20);
  }
  //turn pixels off one by one
  for (uint16_t i=0; i<trellis.pixels.numPixels(); i++) {
    trellis.pixels.setPixelColor(i, 0x000000);
    trellis.pixels.show();
    delay(20);
  }
  
  
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t Neotrellis::Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return trellis.pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return trellis.pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return trellis.pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  return 0;
}



//define a callback for key presses
TrellisCallback pad_callback(keyEvent evt){
  // Check is the pad pressed?
  if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING) {
    neo.trellis.pixels.setPixelColor(evt.bit.NUM, neo.Wheel(map(evt.bit.NUM, 0, neo.trellis.pixels.numPixels(), 0, 255))); //on rising
    
    //set corresponding button states array element to 1, so we know which button has been pressed
    neo.button_presses[evt.bit.NUM] = 1;
    
    //reset timer for detecting a long press, which will take us to the main menu if held for 1.2secs
    neo.button_hold_counter = 0;
    neo.held_button_id = evt.bit.NUM;


  } else if (evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING) {
    //turn the pad LED off when rel
    neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0);
  
    //if the held button has been released, we dont want to show main menu. Resetting the held button id means we are no longer tracking that long press.
    if(neo.held_button_id == evt.bit.NUM){
      //this bit shouldnt ever get reached
      if(neo.button_hold_counter > 1500){
        Serial.println("Released key was held for over 1.5secs, but wasnt detected properly");
      }else{
        neo.held_button_id = -1;
      }
    }

  
  }

  //update neotrellis
  neo.trellis.pixels.show();

  return 0;
}
