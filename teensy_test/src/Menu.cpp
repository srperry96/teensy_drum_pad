#include "Menu.h"
#include "SharedBits.h"

//use 'screen' to do all the tft screen stuff - included from SharedBits.h

//code in this menu bit should
//listen for call to enter menu mode
//process inputs when in menu mode - do stuff, set lights as needed


Menu menu;

Menu::Menu(){}

//notes:
//allow 20 pixels in y for each line



void Menu::show_main_menu(){
    screen.clear();

    screen.print_text("Main Menu", 0, 0);
    screen.print_text("Need some options here", 10, 20);
    screen.print_text("eg osc menu", 10, 40);
}


void Menu::show_osc_menu(){
    screen.clear();

    screen.print_text("osc mode", 20, 80);
}



//sd card navigator - method for changing which samples are mapped to which pad


//step sequencer


//loop mode
//- possibly 2 channels
//- record + overdub
//- undo?
//- mic on / off

//osc menu options:
//- wave type for osc 1 and osc 2?
//- octave +-
//- back button