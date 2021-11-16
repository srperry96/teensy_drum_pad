#include "AudioBits.h"

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioPlayMemory    sound0;
AudioPlayMemory    sound1;  // six memory players, so we can play
AudioPlayMemory    sound2;  // all six sounds simultaneously
AudioPlayMemory    sound3;
AudioPlayMemory    sound4;
// AudioPlayMemory    sound5;
AudioMixer4        mix1;    // two 4-channel mixers are needed in
AudioMixer4        mix2;    // tandem to combine 6 audio sources

// AudioMixer4 mix3;
// AudioMixer4 mix4;


AudioOutputI2S     headphones;
// AudioOutputAnalog  dac;     // play to both I2S audio board and on-chip DAC

AudioPlaySdWav playWav1;

// Create Audio connections between the components

AudioConnection c1(sound0, 0, mix1, 0);
AudioConnection c2(sound1, 0, mix1, 1);
AudioConnection c3(sound2, 0, mix1, 2);
AudioConnection c4(sound3, 0, mix1, 3);
AudioConnection c5(mix1, 0, mix2, 0);   // output of mix1 into 1st input on mix2

AudioConnection c6(sound4, 0, mix2, 1);
// // AudioConnection c7(sound5, 0, mix2, 2);
AudioConnection c8(mix2, 0, headphones, 0);
AudioConnection c9(mix2, 0, headphones, 1);
// // AudioConnection c10(mix2, 0, dac, 0);

AudioConnection c7(playWav1, 0, mix2, 2);
// AudioConnection c10(playWav1, 1, mix2, 2);

//Attempting to do some SD card stuff here
// AudioConnection patch1(playSdWav1, 0, mix1, 0);
// AudioConnection patch2(mix1, 0, headphones, 0);
// AudioConnection patch3(mix1, 0, headphones, 1);

// Create an object to control the audio shield.
AudioControlSGTL5000 audioShield;


void setup_audio_bits() {
  // Configure the pushbutton pins for pullups.
  // Each button should connect from the pin to GND.
  // pinMode(0, INPUT_PULLUP);
  // pinMode(1, INPUT_PULLUP);
  // pinMode(2, INPUT_PULLUP);
  // pinMode(3, INPUT_PULLUP);
  // pinMode(4, INPUT_PULLUP);
  // pinMode(5, INPUT_PULLUP);

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(10);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  // by default the Teensy 3.1 DAC uses 3.3Vp-p output
  // if your 3.3V power has noise, switching to the
  // internal 1.2V reference can give you a clean signal
  //dac.analogReference(INTERNAL);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);
  mix1.gain(2, 0.4);
  mix1.gain(3, 0.4);
  mix2.gain(1, 0.4);
  mix2.gain(2, 0.4);
  mix2.gain(3, 0.4);


  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);

  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }

}

void sample_switch(int pad_num) {
  switch(pad_num){
    case 0 : sound0.play(AudioSampleSnare);
             Serial.println("play 0");
              break;
    case 1 : sound1.play(AudioSampleTomtom);
             Serial.println("play 1");
              break;
    case 2 : sound2.play(AudioSampleHihat);
             Serial.println("play 2");
              break;
    case 3 : sound3.play(AudioSampleKick);
             Serial.println("play 3");
              break;
    case 4 : sound4.play(AudioSampleAshyknee_kick);//AudioSampleCashregister);
             Serial.println("play 4");
             break;
    case 5 : playFile("kicks/kick2.wav");
             break;
    case 6 : playFile("kicks/FriedChicken.wav");
             break;
    case 7 : playFile("kicks/Boom_Bap_Kick_97.wav");
             break;
    case 8 : playFile("claps/clap1.wav");
             break;
    case 9 : playFile("snares/808_SNARE_20.wav");
             break;
    case 10 : playFile("snares/808_SNARE_13.wav");
             break;
    case 11 : playFile("snares/808_SNARE_12.wav");
             break;
    case 12 : playFile("hats/closedhat1.wav");
             break;
    case 13 : playFile("hats/CH1.wav");
             break;
    case 14 : playFile("openhats/openhat1.wav");
             break;
    case 15 : playFile("openhats/HHOPEN808.wav");
             break;
    default : Serial.print("Hit the default case in switch statement. Number was: ");
              Serial.println(pad_num);
              break;
  }
}


void playFile(const char *filename){
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(25);

  // Simply wait for the file to finish playing.
  /////COMMENTED THIS WHILE LOOP OUT AS IT BLOCKS THE REST OF THE LOOP FROM RUNNING
  // while (playWav1.isPlaying()) {
    // uncomment these lines if you audio shield
    // has the optional volume pot soldered
    //float vol = analogRead(15);
    //vol = vol / 1024;
    // sgtl5000_1.volume(vol);
  // }
}