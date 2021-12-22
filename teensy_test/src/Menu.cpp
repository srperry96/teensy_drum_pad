#include "Menu.h"
#include "SharedBits.h"

//use 'screen' to do all the tft screen stuff - included from SharedBits.h

//code in this menu bit should
//listen for call to enter menu mode
//process inputs when in menu mode - do stuff, set lights as needed


//sd card navigator - method for changing which samples are mapped to which pad


//step sequencer


//loop mode
//- possibly 2 channels
//- record + overdub
//- undo?
//- mic on / off

Menu menu;

Menu::Menu(){}

//notes:
//allow 20 pixels in y for each line


String main_menu_items[6] = {"Drum Pad", "Set Drum Pad Sounds", "Oscillator", "Looper", "Step Sequencer", "Metronome"};
String osc_menu_items[4] = {"Wave Shape", "Low Pass Filter", "Detune", "Distortion"};



void Menu::show_main_menu(){
    show_trellis_navigation_controls();

    mode = MENU_MODE_MAIN;

    String temp;
    menu_length = sizeof(main_menu_items) / 16; // divide by 16 bits (size per string)
    screen.clear();

    screen.print_text("MAIN MENU", 100, 0);
    screen.print_text("--------------------------", 3, 20);


    //COULD CHANGE THIS SO MENU IS PRINTED 1 CHARACTED SPACE IN FROM LEFT, THEN ">" IS DRAWN IN THE SPACE. 
    //INSTEAD OF CLEARING SCREEN, PRINT " " TO OVERWRITE IT? OR DRAW SOLID RECTANGLE OVER IT

    for(uint8_t i = 0; i < sizeof(main_menu_items); i++){
        temp = main_menu_items[i];

        //add an arrow to show the selected menu item
        if(i == selection_id){
            temp.append(" <");
        }

        screen.print_text(temp.c_str(), 40, (i * 20) + 40);
    }

    screen.print_text("Red = back to drum pad", 0, 200);
}


TrellisCallback oscillator_callback(keyEvent evt){
//alter lights to give some feedback when a button is pressed / released
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        if(evt.bit.NUM == 0){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 255, 0, 0);
        }else if(evt.bit.NUM == 1){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, 255);
        }else if((evt.bit.NUM == 2) || (evt.bit.NUM == 3)){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 255, 102, 0);
        }else{
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 255, 0);
        }
    }else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
        if(evt.bit.NUM == 0){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 50, 0, 0);
        }else if(evt.bit.NUM == 1){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, 50);
        }else if((evt.bit.NUM == 2) || (evt.bit.NUM == 3)){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 50, 20, 0);
        }else{
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, 0);
        }
    }

    neo.trellis.pixels.show();


//now do stuff with the input (only when the button is pressed, not on release)
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        menu.process_oscillator_input(evt.bit.NUM);
    }

    return 0;
}


void Menu::osc_mode(){
    mode = MENU_MODE_OSCILLATOR;

    //detach old callback, attach oscillator callback, set neotrellis keys for oscillator mode
    for(int i = 0; i < NEO_TRELLIS_NUM_KEYS; i++){
        neo.trellis.unregisterCallback(i);
        neo.trellis.registerCallback(i, oscillator_callback);
        if(i >= 4){
            neo.trellis.pixels.setPixelColor(i, 0, 0, 0);
        }
    }

    neo.trellis.pixels.setPixelColor(0, 50, 0, 0);
    neo.trellis.pixels.setPixelColor(1, 0, 0, 50);
    neo.trellis.pixels.setPixelColor(2, 50, 20, 0);
    neo.trellis.pixels.setPixelColor(3, 50, 20, 0);

    neo.trellis.pixels.show();

    show_oscillator_menu();
}

void Menu::process_oscillator_input(uint16_t key){
    if(key == 0){
        //stop oscillator, then show main menu
        if(wavegen.wave_playing){
            wavegen.stop_osc1();
            wavegen.stop_osc2();
            wavegen.wave_playing = false;
        }
        menu.show_main_menu();
    }else if(key == 1){
        //CYCLE THROUGH WAVESHAPES

    }else if(key == 2){
        //OCTAVE DOWN

    }else if(key == 3){
        //OCTAVE UP

    }else if(key >= 4){
        //start oscillator if not already playing
        if(!wavegen.wave_playing){
            wavegen.start_osc1();
            wavegen.start_osc2();
            wavegen.wave_playing = true;
          }
          //play corresponding note
          wavegen.play_pad_note(key);
    }
   
    //key 0 = back to main menu
    //key 1 = cycle through waveshapes, updates image on screen
    //key 2,3 = octave down/up
    //key 4-15 = notes c through b, played like a keyboard - play oscillator when held, turn off when released


}



void Menu::show_oscillator_menu(){
    screen.clear();
    screen.print_text("Oscillator", 100, 0);
    screen.print_text("--------------------------", 3, 20);
    
    for(int i = 0; i < 4; i++){
        screen.print_text(osc_menu_items[i].c_str(), 20, 40 + (i * 40));
    }
}




TrellisCallback navigation_callback(keyEvent evt){

/////changing pad LED brightness to give user some feedback when they press a button
    //on press, make the pressed button a bit brighter
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        if((evt.bit.NUM == NAV_UP) || (evt.bit.NUM ==NAV_LEFT) || (evt.bit.NUM == NAV_RIGHT) || (evt.bit.NUM == NAV_DOWN)){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, 255);
        }else if(evt.bit.NUM == NAV_SELECT){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 255, 0);
        }else if(evt.bit.NUM == NAV_BACK){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 255, 0, 0);
        }

    //decrease brightness on release
    }else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
        if((evt.bit.NUM == NAV_UP) || (evt.bit.NUM == NAV_LEFT) || (evt.bit.NUM == NAV_RIGHT) || (evt.bit.NUM == NAV_DOWN)){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, 50);
        }else if(evt.bit.NUM == NAV_SELECT){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 50, 0);
        }else if(evt.bit.NUM == NAV_BACK){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 50, 0, 0);
        }
    }
    neo.trellis.pixels.show();


/////now actually processing the input value (when button is pressed, not on release)
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        switch(menu.mode){
            case MENU_MODE_MAIN :           menu.process_main_menu(evt.bit.NUM);
                                            break;

            case MENU_MODE_SET_SAMPLES :    //menu.process_set_sample_menu(evt.bit.NUM);
                                            break;

            // case MENU_MODE_OSCILLATOR - shouldnt need navigation for osc, but need a button for octave + and -, a back button, and a waveshape button that cycles through waveforms
            // case MENU_MODE_LOOPER - depending on looper complexity, may or may not need navigation. need record button, play pause, delete loop, back button
            // case MENU_MODE_STEP_SEQUENCER - will need some decent navigation here for choosing steps, choosing sample to play etc

            //case MENU_MODE_DRUM_PAD - dont use navigation in drum pad mode
            //case MENU_MODE_METRONOME - wont need navigation, but will need keys to set tempo +-, play/pause button and back button

        }

    }

    return 0;
}


void Menu::process_main_menu(uint16_t key){
   
    //move up the menu on screen (down in numbers)
    if(key == NAV_UP){
        if(menu.selection_id > 0){
            menu.selection_id--;
            menu.show_main_menu();
        }
        
    //move down the menu on screen (up in numbers)
    }else if(key == NAV_DOWN){
        if(menu.selection_id < menu.menu_length - 1){
            menu.selection_id++;
            menu.show_main_menu();
        }
    
    //process selected menu option
    }else if(key == NAV_SELECT){
        switch(menu.selection_id + 1){ //+ 1 required here as MENU_MODE_MAIN is the 0 value in our main menu array
            case MENU_MODE_DRUM_PAD :       menu.drum_pad_mode();
                                            break;
            case MENU_MODE_SET_SAMPLES :    menu.set_samples_mode();
                                            break;
            case MENU_MODE_OSCILLATOR :     menu.osc_mode();
                                            break;
            case MENU_MODE_LOOPER :         menu.looper_mode();
                                            break;
            case MENU_MODE_STEP_SEQUENCER : menu.step_sequencer_mode();
                                            break;
            case MENU_MODE_STEP_METRONOME : menu.metronome_mode();
                                            break;
        }

    
    //go back to drum pad if in the main menu
    }else if(key == NAV_BACK){
        menu.drum_pad_mode();
    }
}




void Menu::set_samples_mode(){
    screen.clear();
    screen.print_text("set samples mode to be added", 0, 0);
}


void Menu::step_sequencer_mode(){
    screen.clear();
    screen.print_text("step seq mode to be added", 0, 0);
}

void Menu::looper_mode(){
    screen.clear();
    screen.print_text("looper mode to be added", 0, 0);
}

void Menu::metronome_mode(){
    screen.clear();
    screen.print_text("Metronome mode to be added", 0, 0);
}


void Menu::drum_pad_mode(){
    mode = MENU_MODE_DRUM_PAD;

    //detach old callback, attach drumpad callback, clear neotrellis keys
    for(int i = 0; i < NEO_TRELLIS_NUM_KEYS; i++){
        neo.trellis.unregisterCallback(i);
        neo.trellis.registerCallback(i, pad_callback);
        neo.trellis.pixels.setPixelColor(i, 0, 0, 0);
    }
    neo.trellis.pixels.show();

    show_drum_pad_screen();
}

void Menu::show_drum_pad_screen(){
    screen.clear();
    screen.print_text("Big Beatz Machine", 60, 0);
    screen.print_text("--------------------------", 3, 20);
    screen.print_text("Hold any pad for main menu (>1.2s)", 20, 60);
}



void Menu::show_trellis_navigation_controls(){
    //remove the drum pad callback function, attach the menu navigation callback function
    for(int i = 0; i < NEO_TRELLIS_NUM_KEYS; i++){
        neo.trellis.unregisterCallback(i);
        neo.trellis.registerCallback(i, navigation_callback);
        neo.trellis.pixels.setPixelColor(i, 0, 0, 0);
    }

    //red = back button on pad 0
    neo.trellis.pixels.setPixelColor(0, 50, 0, 0);

    //blue = arrow keys for navigation up, down, left, right
    neo.trellis.pixels.setPixelColor(6, 0, 0, 50);
    neo.trellis.pixels.setPixelColor(9, 0, 0, 50);
    neo.trellis.pixels.setPixelColor(11, 0, 0, 50);
    neo.trellis.pixels.setPixelColor(14, 0, 0, 50);

    //green = select button in centre of arrow keys
    neo.trellis.pixels.setPixelColor(10, 0, 50, 0);

    //update the keypad
    neo.trellis.pixels.show();
}
