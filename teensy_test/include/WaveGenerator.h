#ifndef __WAVEGENERATOR_H__
#define __WAVEGENERATOR_H__

#include <Audio.h>

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


        void set_filter_freq(int freq);

        void play_pad_note(int btn);


        bool wave_playing;

};


//these are instantiated in AudioBits.h as they need to be there for the patches to work properly
extern AudioSynthWaveform osc1;
extern AudioSynthWaveform osc2;
extern AudioFilterStateVariable filter1;


#endif