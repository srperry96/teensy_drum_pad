#include <TFTScreenBits.h>
#include "SharedBits.h"

ILI9341_t3 TFTScreen::tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
TFTScreen screen;


TFTScreen::TFTScreen(){
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(bg_color);
    tft.setTextColor(text_color);
    tft.setTextSize(2);

    Serial.println("Setup Screen"); 
}

//TODO: Function that updates the screen based on the mode we are in
//      This should be called from the main loop at a fixed rate (20Hz or so should do - every 50ms)
void TFTScreen::update(){
    switch(menu.mode){
        case MENU_MODE_MAIN             :   
                                            break;
        case MENU_MODE_DRUM_PAD         :   
                                            break;
        case MENU_MODE_SET_SAMPLES      :   
                                            break;
        case MENU_MODE_OSCILLATOR       :   menu.show_osc_values();
                                            break;
        case MENU_MODE_LOOPER           :   
                                            break;
        case MENU_MODE_STEP_SEQUENCER   :   
                                            break;
        case MENU_MODE_METRONOME        :   
                                            break;
    }


}

void TFTScreen::clear(){
    tft.fillScreen(bg_color);
}

void TFTScreen::print_text(const char* text, uint8_t x, uint8_t y){
    tft.setCursor(x,y);
    tft.println(text);
}

void TFTScreen::cover_letter_with_rectangle(uint8_t x, uint8_t y){
    tft.fillRect(x, y, 15, 18, bg_color);
}

void TFTScreen::cover_line_end(uint8_t x, uint8_t y){
    tft.fillRect(x, y, 200, 18, bg_color);
}