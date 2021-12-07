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

AudioPlayMemory metronome_click;


//wav player connections
AudioConnection wav1_left(playWav1, 0, mix1, 0);
AudioConnection wav1_right(playWav1, 1, mix1, 1);

AudioConnection wav2_left(playWav2, 0, mix1, 2);
AudioConnection wav2_right(playWav2, 1, mix1, 3);

AudioConnection wav3_left(playWav3, 0, mix2, 0);
AudioConnection wav3_right(playWav3, 1, mix2, 1);



//waveform generator bits
AudioSynthWaveform osc1;
AudioSynthWaveform osc2;

AudioFilterStateVariable filter1;

AudioMixer4 osc_mixer;


//wave generator + filter patches
AudioConnection osc1_to_osc_mix(osc1, 0, osc_mixer, 0);
AudioConnection osc2_to_osc_mix(osc2, 0, osc_mixer, 1);

AudioConnection osc_mix_to_filter(osc_mixer, 0, filter1, 0);
AudioConnection filter_to_mixer(filter1, 0, mix2, 3);


//metronome patch
AudioConnection metronome_patch(metronome_click, 0, mix2, 2);



//output mixer connections
AudioConnection mix1_left_to_out(mix1, 0, mix_out, 0);
AudioConnection mix1_right_to_out(mix1, 1, mix_out, 1);

AudioConnection mix2_left_to_out(mix2, 0, mix_out, 2);
AudioConnection mix2_right_to_out(mix2, 1, mix_out, 3);


AudioConnection output_left(mix_out, 0, headphones, 0);
AudioConnection output_right(mix_out, 0, headphones, 1);


// Create an object to control the audio shield.
AudioControlSGTL5000 audioShield;


uint16_t current_volume = 500;

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

  Serial.println("Setup Audio");
}


void play_pad_sample(int pad_num) {

  if((pad_num < 0) || (pad_num > 15)){
    Serial.print("Pad number out of range: ");
    Serial.println(pad_num);
    return;
  }else{
    play_file(pad_sample_paths[pad_num].c_str());
  }
}


void play_file(const char *filename){
  // Serial.print("Playing file: ");
  // Serial.println(filename);

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

void play_metronome(){
  metronome_click.play(AudioSampleMetronome_click);
}


void set_volume(int volume){
  current_volume = volume;
  float vol = volume / 1024.0;
  audioShield.volume(vol);
}