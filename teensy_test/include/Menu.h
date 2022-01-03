#ifndef __MENU_H__
#define __MENU_H__

#include "Arduino.h"

#define MENU_MODE_MAIN 0
#define MENU_MODE_DRUM_PAD 1
#define MENU_MODE_SET_SAMPLES 2
#define MENU_MODE_OSCILLATOR 3
#define MENU_MODE_LOOPER 4
#define MENU_MODE_STEP_SEQUENCER 5
#define MENU_MODE_METRONOME 6

#define NAV_UP 6
#define NAV_DOWN 14
#define NAV_LEFT 9
#define NAV_RIGHT 11
#define NAV_BACK 0
#define NAV_SELECT 10

#define BTN_BRIGHT 50
#define BTN_DIM 25

class Menu{
    public:
        Menu();

        void show_trellis_navigation_controls();
        
        void main_menu_mode();
        void process_main_menu(uint16_t key);
        void move_menu(uint8_t direction);


        void drum_pad_mode();


        void osc_mode();
        void show_oscillator_menu();
        void show_osc_values();
        void process_oscillator_input(uint16_t key, uint8_t stroke);

                
        void looper_mode();
        void process_looper_input(uint16_t key, uint8_t stroke);


        void set_samples_mode();


        void step_sequencer_mode();


        void metronome_mode();


        uint8_t mode = MENU_MODE_DRUM_PAD;
        uint8_t selection_id = 0;
        uint8_t menu_length = 3;
};




#endif