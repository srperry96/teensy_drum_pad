#include "Menu.h"
#include "SharedBits.h"
#include "AudioBits.h"

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


String main_menu_items[6] = {"Drum Pad", "Set Drum Pad Sounds", "Oscillator", "(Rec)Looper", "Step Sequencer", "Metronome"};
String osc_menu_items[5] = {"Wave Shape", "Octave", "LP Filter", "Osc2 Detune", "Overdrive"};



void Menu::main_menu_mode(){
    mode = MENU_MODE_MAIN;

    show_trellis_navigation_controls();

    String temp;
    menu_length = sizeof(main_menu_items) / 16; // divide by 16 bits (size per string)
    screen.clear();

    screen.print_text("MAIN MENU", 100, 0);
    screen.print_text("--------------------------", 3, 20);

    for(uint8_t i = 0; i < menu_length; i++){
        temp = main_menu_items[i];
        screen.print_text(temp.c_str(), 40, (i * 20) + 40);
    }

    screen.print_text("Red = back to drum pad", 0, 200);

    //draw menu selection arrow
    screen.print_text(">", 10, (selection_id * 20) + 40);
}

void Menu::move_menu(uint8_t direction){
    if(direction == 0){
        screen.cover_letter_with_rectangle(10, ((selection_id + 1) * 20) + 40);
    }else{
        screen.cover_letter_with_rectangle(10, ((selection_id - 1) * 20) + 40);
    }
    screen.print_text(">", 10, (selection_id * 20) + 40);
}

TrellisCallback oscillator_callback(keyEvent evt){
//alter lights to give some feedback when a button is pressed / released
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        if(evt.bit.NUM == 0){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, BTN_BRIGHT, 0, 0);
        }else if(evt.bit.NUM == 1){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, BTN_BRIGHT);
        }else if((evt.bit.NUM == 2) || (evt.bit.NUM == 3)){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, BTN_BRIGHT, BTN_BRIGHT/2, 0);
        }else{
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, BTN_BRIGHT, 0);
        }
    }else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
        if(evt.bit.NUM == 0){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, BTN_DIM, 0, 0);
        }else if(evt.bit.NUM == 1){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, BTN_DIM);
        }else if((evt.bit.NUM == 2) || (evt.bit.NUM == 3)){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, BTN_DIM, BTN_DIM/2, 0);
        }else{
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, 0);
        }
    }

    neo.trellis.pixels.show();


//now do stuff with the input (only when the button is pressed, not on release)
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        menu.process_oscillator_input(evt.bit.NUM, SEESAW_KEYPAD_EDGE_RISING);
    }else{
        menu.process_oscillator_input(evt.bit.NUM, SEESAW_KEYPAD_EDGE_FALLING);
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

    neo.trellis.pixels.setPixelColor(0, BTN_DIM, 0, 0);
    neo.trellis.pixels.setPixelColor(1, 0, 0, BTN_DIM);
    neo.trellis.pixels.setPixelColor(2, BTN_DIM, BTN_DIM/2, 0);
    neo.trellis.pixels.setPixelColor(3, BTN_DIM, BTN_DIM/2, 0);

    neo.trellis.pixels.show();

    //reset previous osc values array to ensure we print them out this time (divide by 4 bytes per float to get length of the array)
    for(uint16_t i = 0; i < sizeof(wavegen.prev_osc_vals) / 4; i++){
        wavegen.prev_osc_vals[i] = -1.0;
    }

    show_oscillator_menu();
}


void Menu::show_oscillator_menu(){
    screen.clear();
    screen.print_text("Oscillator (2ch)", 70, 0);
    screen.print_text("--------------------------", 3, 20);
    
    for(uint8_t i = 0; i < (sizeof(osc_menu_items) / 16); i++){
        screen.print_text(osc_menu_items[i].c_str(), 20, (i * 20) + 40);
    }
    show_osc_values();
}


void Menu::process_oscillator_input(uint16_t key, uint8_t stroke){
    //Return to main menu, stopping oscillator
    if(key == 0){
        //if oscillators are playing, stop them
        if(wavegen.wave_playing){
            wavegen.stop_osc1();
            wavegen.stop_osc2();
            wavegen.wave_playing = false;
        }
        menu.main_menu_mode();
        return;
    
    //Cycle through the wave shape options
    }else if((key == 1) && (stroke == SEESAW_KEYPAD_EDGE_RISING)){
        //increment waveshape ID, ensuring we stay within the limits of the list of waveshapes
        wavegen.waveshape++;
        if(wavegen.waveshape > wavegen.num_waveshapes - 1){
            wavegen.waveshape = 0;
        }
        
        //if oscillator is playing, we must restart it for the change to take effect
        if(wavegen.wave_playing){
            wavegen.start_osc1();
            wavegen.start_osc2();
        }

    //Octave down - limit of octave 0 for obvious reasons
    }else if((key == 2) && (stroke == SEESAW_KEYPAD_EDGE_RISING)){
        if(wavegen.octave > 0){
            wavegen.octave--;
        }

    //Octave up - limit of 7 stops us going to horribly high pitches
    }else if((key == 3) && (stroke == SEESAW_KEYPAD_EDGE_RISING)){
        if(wavegen.octave < 7){
            wavegen.octave++;
        }

    //Play the corresponding note
    }else if(key >= 4){
        //if the key is pushed, we want a note to play
        if(stroke == SEESAW_KEYPAD_EDGE_RISING){
            if(!wavegen.wave_playing){
                wavegen.start_osc1();
                wavegen.start_osc2();
                wavegen.wave_playing = true;
            }
            wavegen.play_pad_note(key);
        
        //else if the key is being released, we want to stop playing? STILL UNDECIDED
        }else{
//COMMENT OUT THE BELOW FEW LINES TO MAKE THE OSCILLATORS TURN OFF WHEN THE KEY IS RELEASED (RESULTS IN A LOT OF CRACKLING SOUNDS)
//--- to make this work properly, would need to keep track of the note thats playing. If that note is the key that is released, then we stop. 
//                                                                                    Otherwise continue playing, as a new note has been pressed.
//ie:
    //if osc1 freq == CORRESPONDING FREQUENCY FOR THAT BUTTON{stop oscillators}; else do nothing;
    //this will not work for frequencies in different octaves. easiest way will probably be doable once we have a lookup table for all the notes C0 through C7
       
            // wavegen.stop_osc1();
            // wavegen.stop_osc2();
            // wavegen.wave_playing = false;
        }
    }

    //update all the values on screen
    show_osc_values();
}


//draw values for oscillator params
void Menu::show_osc_values(){
    const uint16_t xpos = 160;
    char temp[16];
    
    if(wavegen.waveshape != wavegen.prev_osc_vals[0]){
        switch(wavegen.waveshape){
            //sin
            case 0 :    sprintf(temp, "Sin ~~");
                        break;
            //sawtooth
            case 1 :    sprintf(temp, "Saw /|/|");
                        break;
            //square
            case 2 :    sprintf(temp, "Square |_|-|");
                        break;
            //triangle
            case 3 :    sprintf(temp, "Triangle /\\/\\");
                        break;
            //sawtooth reverse
            case 4 :    sprintf(temp, "Rev Saw |\\|\\");
                        break;
        }

        screen.cover_line_end(xpos, 40);
        screen.print_text(temp, xpos, 40);

        wavegen.prev_osc_vals[0] = wavegen.waveshape;
    }

    if(wavegen.octave != wavegen.prev_osc_vals[1]){
        sprintf(temp, "%d", wavegen.octave);
        screen.cover_line_end(xpos, 60);
        screen.print_text(temp, xpos, 60);

        wavegen.prev_osc_vals[1] = wavegen.octave;
    }

    if(wavegen.low_pass_val != wavegen.prev_osc_vals[2]){
        sprintf(temp, "%d", wavegen.low_pass_val);
        screen.cover_line_end(xpos, 80);
        screen.print_text(temp, xpos, 80);

        wavegen.prev_osc_vals[2] = wavegen.low_pass_val;
    }

    if(wavegen.detune_val != wavegen.prev_osc_vals[3]){
        sprintf(temp, "%.3f", wavegen.detune_val);
        screen.cover_line_end(xpos, 100);
        screen.print_text(temp, xpos, 100);

        wavegen.prev_osc_vals[3] = wavegen.detune_val;
    }

    if(wavegen.overdrive_val != wavegen.prev_osc_vals[4]){
        sprintf(temp, "%d", wavegen.overdrive_val);
        screen.cover_line_end(xpos, 120);
        screen.print_text(temp, xpos, 120);

        wavegen.prev_osc_vals[4] = wavegen.overdrive_val;
    }

}



TrellisCallback navigation_callback(keyEvent evt){

/////changing pad LED brightness to give user some feedback when they press a button
    //on press, make the pressed button a bit brighter
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        if((evt.bit.NUM == NAV_UP) || (evt.bit.NUM ==NAV_LEFT) || (evt.bit.NUM == NAV_RIGHT) || (evt.bit.NUM == NAV_DOWN)){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, BTN_BRIGHT);
        }else if(evt.bit.NUM == NAV_SELECT){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, BTN_BRIGHT, 0);
        }else if(evt.bit.NUM == NAV_BACK){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, BTN_BRIGHT, 0, 0);
        }

    //decrease brightness on release
    }else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
        if((evt.bit.NUM == NAV_UP) || (evt.bit.NUM == NAV_LEFT) || (evt.bit.NUM == NAV_RIGHT) || (evt.bit.NUM == NAV_DOWN)){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, BTN_DIM);
        }else if(evt.bit.NUM == NAV_SELECT){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, BTN_DIM, 0);
        }else if(evt.bit.NUM == NAV_BACK){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, BTN_DIM, 0, 0);
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
            menu.move_menu(0);
        }
    //move down the menu on screen (up in numbers)
    }else if(key == NAV_DOWN){
        if(menu.selection_id < menu.menu_length - 1){
            menu.selection_id++;
            menu.move_menu(1);
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
            case MENU_MODE_METRONOME :      menu.metronome_mode();
                                            break;
        }
    //go back to drum pad mode
    }else if(key == NAV_BACK){
        menu.drum_pad_mode();
    }
}




void Menu::set_samples_mode(){
    // mode = MENU_MODE_SET_SAMPLES;
    screen.clear();
    screen.print_text("set samples mode to be added", 0, 0);
}


void Menu::step_sequencer_mode(){
    // mode = MENU_MODE_STEP_SEQUENCER;
    screen.clear();
    screen.print_text("step seq mode to be added", 0, 0);
}



TrellisCallback looper_callback(keyEvent evt){
//alter lights to give some feedback when a button is pressed / released
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        if(evt.bit.NUM == 0){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, BTN_BRIGHT, 0, 0);
        }else if(evt.bit.NUM == 1){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, BTN_BRIGHT);
        }
    }else if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_FALLING){
        if(evt.bit.NUM == 0){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, BTN_DIM, 0, 0);
        }else if(evt.bit.NUM == 1){
            neo.trellis.pixels.setPixelColor(evt.bit.NUM, 0, 0, BTN_DIM);
        }
    }

    neo.trellis.pixels.show();


//now do stuff with the input (only on press, not release)
    if(evt.bit.EDGE == SEESAW_KEYPAD_EDGE_RISING){
        menu.process_looper_input(evt.bit.NUM, evt.bit.EDGE);
    }

    return 0;
}



void Menu::looper_mode(){
    mode = MENU_MODE_LOOPER;

    //attach new callback to keypad
    for(int i = 0; i < NEO_TRELLIS_NUM_KEYS; i++){
        neo.trellis.unregisterCallback(i);
        neo.trellis.registerCallback(i, looper_callback);
        neo.trellis.pixels.setPixelColor(i, 0, 0, 0);
    }

    neo.trellis.pixels.setPixelColor(0, BTN_DIM, 0, 0);
    neo.trellis.pixels.setPixelColor(1, 0, 0, BTN_DIM);
    neo.trellis.pixels.setPixelColor(2, 0, BTN_DIM, BTN_DIM);
    neo.trellis.pixels.setPixelColor(3, BTN_DIM, 0, BTN_DIM);


    neo.trellis.pixels.show();

    //show the looper menu
    screen.clear();
    screen.print_text("Test recording", 10, 30);
    screen.print_text("press 2 to record", 10, 60);
    screen.print_text("press 3 to stop", 10, 90);
    screen.print_text("press 4 to playback", 10, 120);
}



//LOOPER DEVELOPMENT CODE BELOW
//0 is stop mode
//1 is record mode
//2 is play mode

void Menu::process_looper_input(uint16_t key, uint8_t stroke){
    if(key == 0){
        menu.main_menu_mode();
        return;
    //1 to record
    }else if(key == 1){
            Serial.println("Record Button Press");
            if (sound.loopermode == 2) sound.stopPlaying();
            if (sound.loopermode == 0) sound.startRecording();

    //2 to stop
    }else if(key == 2){
        Serial.println("Stop Button Press");
        if (sound.loopermode == 1) sound.stopRecording();
        if (sound.loopermode == 2) sound.stopPlaying();

    //3 to playback
    }else if(key == 3){
        Serial.println("Play Button Press");
        if (sound.loopermode == 1) sound.stopRecording();
        if (sound.loopermode == 0) sound.startPlaying();
    }
}







void Menu::metronome_mode(){
    // mode = MENU_MODE_METRONOME;
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

    //show drum pad screen
    screen.clear();
    screen.print_text("Big Beatz Machine", 60, 0);
    screen.print_text("--------------------------", 3, 20);
    screen.print_text("Hold any pad for main menu (>1.2s)", 20, 60);
}

void Menu::process_drum_pad_input(){
    //check pad states, play corresponding sample if needed
    for(uint8_t btn_count = 0; btn_count < 16; btn_count++){
      if(neo.button_presses[btn_count] == 1){
          sound.play_pad_sample(btn_count);

        //reset the button state so we dont double play the sample
        neo.button_presses[btn_count] = 0;
      }
    }
}


void Menu::show_trellis_navigation_controls(){
    //remove the drum pad callback function, attach the menu navigation callback function
    for(int i = 0; i < NEO_TRELLIS_NUM_KEYS; i++){
        neo.trellis.unregisterCallback(i);
        neo.trellis.registerCallback(i, navigation_callback);
        neo.trellis.pixels.setPixelColor(i, 0, 0, 0);
    }

    //red = back button on pad 0
    neo.trellis.pixels.setPixelColor(0, BTN_DIM, 0, 0);

    //blue = arrow keys for navigation up, down, left, right
    neo.trellis.pixels.setPixelColor(6, 0, 0, BTN_DIM);
    neo.trellis.pixels.setPixelColor(9, 0, 0, BTN_DIM);
    neo.trellis.pixels.setPixelColor(11, 0, 0, BTN_DIM);
    neo.trellis.pixels.setPixelColor(14, 0, 0, BTN_DIM);

    //green = select button in centre of arrow keys
    neo.trellis.pixels.setPixelColor(10, 0, BTN_DIM, 0);

    //update the keypad
    neo.trellis.pixels.show();
}
