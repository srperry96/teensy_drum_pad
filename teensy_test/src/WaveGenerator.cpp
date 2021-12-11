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

#define FILT_CUT_LIMIT 10000

#define FILT_Q_LIM_HIGH 0.7
#define FILT_Q_LIM_LOW 5 //limits between 0.7 and 5

WaveGenerator wavegen;


WaveGenerator::WaveGenerator(){}



//from C0 to B0 (for every octave, we can double freq each time)
float some_note_freqs[12] = {16.35, 17.32, 18.35, 19.45, 20.6, 21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87};

float osc1freq = 80.0, osc2freq = 80.0;

bool wave_playing = false;

void WaveGenerator::start_osc1(){
    osc1.amplitude(0.4);
    osc1.frequency(osc1freq);
    osc1.begin(WAVEFORM_TRIANGLE);
}

void WaveGenerator::start_osc2(){
    osc2.amplitude(0.4);
    osc2.frequency(osc2freq);
    osc2.begin(WAVEFORM_TRIANGLE);
}

void WaveGenerator::set_freq(int note){
    // int scaled = map(freq, 0, 1023, 0, 11);
    int octave = 2;

    float freq_new = some_note_freqs[note];

    //double the frequency for each octave we want to go up
    //this may be a slightly hacky way to do it. Should probs add a lookup table with all notes at some point
    for(uint8_t i = 0; i < octave; i++){
        freq_new *= 2;
    }

    osc1.frequency(freq_new);
    // osc2.frequency(freq_new);

    osc1freq = freq_new;
    // osc2freq = freq_new;
}

void WaveGenerator::set_filter_freq(int freq){
    int scaled = map(freq, 0, 1023, 0, 8000);
    filter1.frequency(scaled);
}

void WaveGenerator::stop_osc1(){
    osc1.amplitude(0);
}

void WaveGenerator::stop_osc2(){
    osc2.amplitude(0);
}

void WaveGenerator::set_osc2_detune(int detune){
    int scaled = map(detune, 0, 1023, 0, 100);
    float scaled2 = scaled / 100.0;
    float detuneVal = 1.0 - (0.04 * scaled2);

    
    osc2freq = osc1freq * detuneVal;

    osc2.frequency(osc2freq);

}

void WaveGenerator::play_pad_note(int btn){

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
        default:    break;   
    }
}
