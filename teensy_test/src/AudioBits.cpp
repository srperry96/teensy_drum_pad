#include "AudioBits.h"

//Mixers to take in wav file signals
AudioMixer4        mix1;
AudioMixer4        mix2;

//output mixer
AudioMixer4        mix_out;


//Output to headphone jack on teensy audio board
AudioOutputI2S     headphones;

//Setup wav players (max 3 to avoid parallel SD card read audio glitches)
AudioPlaySdWav playWav1;
AudioPlaySdWav playWav2;
AudioPlaySdWav playWav3;


//wav player connections
AudioConnection wav1_left(playWav1, 0, mix1, 0);
AudioConnection wav1_right(playWav1, 1, mix1, 1);

AudioConnection wav2_left(playWav2, 0, mix1, 2);
AudioConnection wav2_right(playWav2, 1, mix1, 3);

AudioConnection wav3_left(playWav3, 0, mix2, 0);
AudioConnection wav3_right(playWav3, 1, mix2, 1);

//CANT DO 4 WAV PLAYERS, BUT LEAVING THESE CONNECTIONS HERE AS A REMINDER THAT THERE ARE TWO EMPTY INPUT SLOTS ON MIX2
//THIS COULD BE USED FOR A METRONOME OR SOMETHING ELSE DOWN THE LINE??
// AudioConnection wav4_left(playWav4, 0, mix2, 2);
// AudioConnection wav4_right(playWav4, 1, mix2, 3);

//output mixer connections
AudioConnection mix1_left_to_out(mix1, 0, mix_out, 0);
AudioConnection mix1_right_to_out(mix1, 1, mix_out, 1);

AudioConnection mix2_left_to_out(mix2, 0, mix_out, 2);
AudioConnection mix2_right_to_out(mix2, 1, mix_out, 3);


AudioConnection output_left(mix_out, 0, headphones, 0);
AudioConnection output_right(mix_out, 0, headphones, 1);


// Create an object to control the audio shield.
AudioControlSGTL5000 audioShield;


void setup_audio_bits() {

  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  // reduce the gain on mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mix1.gain(0, 0.4);
  mix1.gain(1, 0.4);
  mix1.gain(2, 0.4);
  mix1.gain(3, 0.4);

  mix2.gain(0, 0.4);
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
    case 0:   playFile("claps/CH1_Clap_09.wav");
              break;
    case 1:   playFile("claps/CH1_Clap_18.wav");
              break;
    case 2:   playFile("claps/clap.wav");
              break;
    case 3:   playFile("hats/muzikhat.wav");
              break;
    case 4:   playFile("kicks/drebd_009.wav");
              break;
    case 5:   playFile("kicks/kick2.wav");
              break;
    case 6:   playFile("kicks/friedchicken.wav");
              break;
    case 7:   playFile("kicks/boombapkick97.wav");
              break;
    case 8:   playFile("claps/clap1.wav");
              break;
    case 9:   playFile("kicks/kick3.wav");
              break;
    case 10:  playFile("snares/snare.wav");
              break;
    case 11:  playFile("snares/incredibly overused snare.wav");
              break;
    case 12:  playFile("snares/06.wav");
              break;
    case 13:  playFile("hats/hat2.wav");
              break;
    case 14:  playFile("openhats/openhat1.wav");
              break;
    case 15:  playFile("snares/snare1.wav");
              break;
    default:  Serial.print("Hit the default case in switch statement. Number was: ");
              Serial.println(pad_num);
              break;
  }
}


void playFile(const char *filename){
  Serial.print("Playing file: ");
  Serial.println(filename);

  //Check for a free wav player to play the sound. Use playWav3 if all are busy
  if(!playWav1.isPlaying()){
    playWav1.play(filename);
  }else if(!playWav2.isPlaying()){
    playWav2.play(filename);
  }else{
    playWav3.play(filename);
  }

  // A brief delay for the library to read WAV info
  delay(25);
}