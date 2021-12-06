#include "WaveGenerator.h"

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
#define B 11

//from C0 to B0 (for every octave, we can double freq each time)
float some_note_freqs[12] = {16.35, 17.32, 18.35, 19.45, 20.6, 21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87};


bool wave_playing = false;

void start_sin(){
    waveform1.amplitude(0.2);
    waveform1.frequency(80);
    waveform1.begin(WAVEFORM_SINE);
}

void set_freq(int freq){
    int scaled = map(freq, 0, 1023, 0, 11);
    int octave = 2;

    float freq_new = some_note_freqs[scaled];

    //double the frequency for each octave we want to go up
    //this may be a slightly hacky way to do it. Should probs add a lookup table with all notes at some point
    for(uint8_t i = 0; i < octave; i++){
        freq_new *= 2;
    }

    waveform1.frequency(freq_new);
}

void set_filter_freq(int freq){
    int scaled = map(freq, 0, 1023, 0, 8000);
    filter1.frequency(scaled);
}

void stop_sin(){
    waveform1.amplitude(0);
}