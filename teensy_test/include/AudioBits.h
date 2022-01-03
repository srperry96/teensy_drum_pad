#ifndef __AUDIOBITS_H__
#define __AUDIOBITS_H__

#include <Audio.h>

class Sound{
    public:
        Sound();

        void update_volume(uint16_t vol);

        void play_pad_sample(int pad_num);

        void play_file(const char *filename);

        void update_pad_sample_filepaths();

        void play_metronome();

        void set_volume(int volume);

        uint16_t current_volume;




        //looper stuff
        void startPlaying();
        void continuePlaying();
        void stopPlaying();

        void startRecording();
        void continueRecording();
        void stopRecording();

        void looper_loop();
        int loopermode = 0;
        File frec;

};

#endif