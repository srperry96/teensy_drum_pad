#ifndef __WAVEGENERATOR_H__
#define __WAVEGENERATOR_H__

#include <Audio.h>


#define WAVEGEN_SINE 0
#define WAVEGEN_SAWTOOTH 1
#define WAVEGEN_SQUARE 2
#define WAVEGEN_TRIANGLE 3
#define WAVEGEN_SAWTOOTH_REVERSE 4// = 6 in the teensy audio library



class WaveGenerator
{
    public:
        WaveGenerator();
        void start_osc1();
        void start_osc2();

        void set_osc2_detune(int detune);

        void set_freq(int note);

        void stop_osc1();
        void stop_osc2();

        void set_overdrive(int val);


        void set_filter_freq(int freq);

        void play_pad_note(int btn);

        uint8_t waveshape = 0;
        const uint8_t num_waveshapes = 5;

        bool wave_playing = false;
        uint8_t octave = 2;

        int low_pass_val;
        int overdrive_val;
        float detune_val;

        float osc1_freq;
        float osc2_freq;

        //from C0 to B0 (for every octave, we can double freq each time)
        float note_freqs[12] = {16.35, 17.32, 18.35, 19.45, 20.6, 21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87};

        //previous used for comparison so we dont constantly update the screen with the same information
        float prev_osc_vals[5] = {-1, -1, -1, -1, -1};


        const uint8_t max_overdrive = 50;
        const uint16_t max_filter_freq = 600;
        const float max_detune = 0.04;

        const uint8_t min_overdrive = 1;
        const uint16_t min_filter_freq = 40;

};


//these are instantiated in AudioBits.h as they need to be there for the patches to work properly
extern AudioSynthWaveform osc1;
extern AudioSynthWaveform osc2;
extern AudioFilterStateVariable osc_filter1;
extern AudioAmplifier osc_dist;
extern AudioAmplifier osc_dist_limiter;

#endif