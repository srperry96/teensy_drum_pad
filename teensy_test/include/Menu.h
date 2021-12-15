#ifndef __MENU_H__
#define __MENU_H__

#include "Arduino.h"

#define MENU_MODE_MAIN 0
#define MENU_MODE_OSC 1
#define MENU_MODE_LOOPER 2


class Menu{
    public:
        Menu();

        void show_main_menu();
        void show_osc_menu();

        // void show_looper_menu();

        uint8_t mode = MENU_MODE_MAIN;
};




#endif