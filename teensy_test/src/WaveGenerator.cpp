#include "WaveGenerator.h"
#include "SharedBits.h"

//defines here to be used with the note freqs array
#define C_0 0
#define C_SHARP_0 1
#define D_0 2
#define D_SHARP_0 3
#define E_0 4
#define F_0 5
#define F_SHARP_0 6
#define G_0 7
#define G_SHARP_0 8
#define A_0 9
#define A_SHARP_0 10
#define B_0 11

WaveGenerator wavegen;


WaveGenerator::WaveGenerator(){
    //set distortion limiter
    osc_dist_limiter.gain(0.4);

    //set up the envelope for oscillator 1/2
    osc_envelope1.attack(40);
    osc_envelope1.release(200);
}


void WaveGenerator::set_freq(int note){
    float freq_new = note_freqs[note];

    //double the frequency for each octave we want to go up
    //this may be a slightly hacky way to do it. Should probs add a lookup table with all notes at some point
    for(uint8_t i = 0; i < octave; i++){
        freq_new *= 2;
    }

    osc1.frequency(freq_new);
    osc1_freq = freq_new;

    osc2_freq = osc1_freq * detune_val;
    osc2.frequency(osc2_freq);
}

void WaveGenerator::set_filter_freq(int freq){
    //* 10 after the map here limits resolution to 10Hz
    int scaled = map(freq, 0, 1023, min_filter_freq / 10, max_filter_freq / 10) * 10;

    //update the value if it has changed
    if(scaled != low_pass_val){
        osc_filter1.frequency(scaled);
        low_pass_val = scaled;
    }
}

void WaveGenerator::set_overdrive(int val){
    int mapped_val = map(val, 0, 1023, min_overdrive, max_overdrive);

    //update the value if it has changed
    if(mapped_val != overdrive_val){
        osc_dist.gain(mapped_val);
        overdrive_val = mapped_val;
    }
}

void WaveGenerator::set_osc2_detune(int detune){
    int scaled = map(detune, 0, 1023, 0, 100);
    float scaled2 = scaled / 100.0;
    float detune_percent = 1.0 - (max_detune * scaled2);

    osc2_freq = osc1_freq * detune_percent;
    osc2.frequency(osc2_freq);

    detune_val = detune_percent;
}

void WaveGenerator::set_note_freq(uint16_t btn){
    current_key = btn;

    switch(btn){
        case 12:    set_freq(C_0);
                    break;
        case 13:    set_freq(C_SHARP_0);
                    break;
        case 14:    set_freq(D_0);
                    break;
        case 15:    set_freq(D_SHARP_0);
                    break;    
        case 8:     set_freq(E_0);
                    break;
        case 9:     set_freq(F_0);
                    break;
        case 10:    set_freq(F_SHARP_0);
                    break;
        case 11:    set_freq(G_0);
                    break;
        case 4:     set_freq(G_SHARP_0);
                    break;
        case 5:     set_freq(A_0);
                    break;
        case 6:     set_freq(A_SHARP_0);
                    break;
        case 7:     set_freq(B_0);
                    break;
    }
}

void WaveGenerator::begin_note(){
    //if oscillator isnt currently playing, use the envelope to begin the note
    if(!wave_playing){
        osc_envelope1.noteOn();
        wave_playing = true;
    }   
}

void WaveGenerator::end_note(){
    osc_envelope1.noteOff();
    wave_playing = false;
    current_key = 0;
}

void WaveGenerator::enable(){
    osc1.amplitude(0.4);
    osc1.frequency(osc1_freq);

    //our wave shape list doesnt use all possible shapes. this remaps 4 to 6 here to include reverse sawtooth
    if(waveshape == 4){
        osc1.begin(6);
    }else{
        osc1.begin(waveshape);
    }

    osc2.amplitude(0.4);
    osc2.frequency(osc2_freq);
    
    if(waveshape == 4){
        osc2.begin(6);
    }else{
        osc2.begin(waveshape);
    }
}

void WaveGenerator::disable(){
    osc1.amplitude(0);
    osc2.amplitude(0);
}

void WaveGenerator::update_effects(int filter_val, int detune_val, int overdrive_val){
    AudioNoInterrupts();
    set_filter_freq(filter_val);
    set_osc2_detune(detune_val);
    set_overdrive(overdrive_val);
    AudioInterrupts();
}

void WaveGenerator::change_waveshape(){
    //increment waveshape ID, ensuring we stay within the limits of the list of waveshapes
    waveshape++;
    if(waveshape > num_waveshapes - 1){
        waveshape = 0;
    }
    //restart the oscillator so the change of waveform takes effect
    enable();
}