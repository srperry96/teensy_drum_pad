#include "AudioBits.h"

//mixers to take in wav file signals
AudioMixer4        mixer_wav1;
AudioMixer4        mixer_wav2; //this is also used for the metronome

//wave generator mixer
AudioMixer4 mixer_osc;

//output mixer
AudioMixer4        mixer_out;

//microphone input from teensy audio board
AudioInputI2S mic_in;

//headphone output on teensy audio board
AudioOutputI2S     headphones;


//Setup wav players (max 3 to avoid parallel SD card read audio glitches)
AudioPlaySdWav playWav1;
AudioPlaySdWav playWav2;
AudioPlaySdWav playWav3;

//metronome click player (stored in raw format in the code)
AudioPlayMemory metronome_click;


//wav player connections
AudioConnection wav1_left(playWav1,  0, mixer_wav1, 0);
AudioConnection wav1_right(playWav1, 1, mixer_wav1, 1);

AudioConnection wav2_left(playWav2,  0, mixer_wav1, 2);
AudioConnection wav2_right(playWav2, 1, mixer_wav1, 3);

AudioConnection wav3_left(playWav3,  0, mixer_wav2, 0);
AudioConnection wav3_right(playWav3, 1, mixer_wav2, 1);

//metronome patch using one of the spare channels on mixer_wav2
AudioConnection metronome_patch(metronome_click, 0, mixer_wav2, 2);

//!!!!1 channel free on mixer_wav2!!!!



//waveform generator bits
AudioSynthWaveform osc1;
AudioSynthWaveform osc2;

//low pass filter for oscillator
AudioFilterStateVariable osc_filter1;

//mixer to combine osc1 and osc2
AudioConnection osc1_to_mixer_osc(osc1, 0, mixer_osc, 0);
AudioConnection osc2_to_mixer_osc(osc2, 0, mixer_osc, 1);

//Distortion (overdrive) into
AudioAmplifier osc_dist;
AudioAmplifier osc_dist_limiter;
AudioConnection mixer_osc_to_dist(mixer_osc, 0, osc_dist, 0);
AudioConnection mixer_osc_to_dist2(osc_dist, 0, osc_dist_limiter, 0);
AudioConnection mixer_osc_to_filter(osc_dist_limiter, 0, osc_filter1, 0);


//output mixer connections
AudioConnection wavs1_to_mixer_out(    mixer_wav1,  0, mixer_out, 0); //wav samples 1 and 2 to output
AudioConnection wavs2_to_mixer_out(    mixer_wav2,  0, mixer_out, 1); //wav samples 3 and metronome to output
AudioConnection wavegen_to_mixer_out(  osc_filter1, 0, mixer_out, 2); //wave generator to output
AudioConnection mic_to_mixer_out(      mic_in,      0, mixer_out, MIC_MIXER_CHANNEL); //microphone straight to output atm


//same signal sent to left and right (= mono audio out)
AudioConnection headphones_out_L(mixer_out, 0, headphones, 0);
AudioConnection headphones_out_R(mixer_out, 0, headphones, 1);


// Create an object to control the audio shield.
AudioControlSGTL5000 audioShield;


Sound::Sound(){
  AudioMemory(12);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  // reduce the gain on wav sample player mixer channels, so more than 1
  // sound can play simultaneously without clipping
  mixer_wav1.gain(0, 0.4); //wav1 left
  mixer_wav1.gain(1, 0.4); //wav1 right
  mixer_wav1.gain(2, 0.4); //wav2 left
  mixer_wav1.gain(3, 0.4); //wav2 right

  mixer_wav2.gain(0, 0.4); //wav3 left
  mixer_wav2.gain(1, 0.4); //wav3 right
  mixer_wav2.gain(2, 0.4); //metronome
  // mixer_wav2.gain(3, 0.4); //currently unused channel


  //set teensy audio board mic as input
  audioShield.inputSelect(AUDIO_INPUT_MIC);
  audioShield.micGain(40);

  //set mic gain to 0 so we dont hear it constantly
  mixer_out.gain(MIC_MIXER_CHANNEL, 0);

  Serial.println("Setup Audio");
}


void Sound::play_pad_sample(int pad_num) {

  if((pad_num < 0) || (pad_num > 15)){
    Serial.print("Pad number out of range: ");
    Serial.println(pad_num);
    return;
  }else{
    play_file(sd_card.pad_sample_paths[pad_num].c_str());
  }
}


void Sound::play_file(const char *filename){
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
  
  // A brief delay for the library to read WAV info -- pnot sure if necessary so commented out for now
  // delay(5);
}

void Sound::play_metronome(){
  metronome_click.play(metronome.AudioSampleMetronome_click);
}


void Sound::set_volume(int volume){
  current_volume = volume;
  float vol = volume / 1024.0;
  audioShield.volume(vol);
}

void Sound::update_volume(uint16_t new_vol){
    //change volume if knob has moved enough (+- 2 accounts for noisy analog readings)
    if(!((current_volume > new_vol - 2) && (current_volume < new_vol + 2))){
      set_volume(new_vol);
    }
}
