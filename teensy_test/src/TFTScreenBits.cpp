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
    tft.fillRect(x, y, ILI9341_TFTHEIGHT, 18, bg_color);
}

void TFTScreen::draw_title(const char* text){
    //draw title
    print_text(text, get_start_x_for_centred_text(text), 0);

    //draw the underline all the way across the screen
    const uint8_t title_underline_y = 20;
    tft.drawLine(0, title_underline_y, ILI9341_TFTHEIGHT, title_underline_y, text_color);
}


uint16_t TFTScreen::get_start_x_for_centred_text(const char* text){
    const uint8_t letter_width = 12;

    //calculation is: middle of screen - half length of title
    uint16_t x_pos = (ILI9341_TFTHEIGHT / 2) - ((letter_width * strlen(text)) / 2);   
    return x_pos;
}



void TFTScreen::draw_colored_pad(uint8_t x, uint8_t y, uint16_t color){
    tft.fillRoundRect(x, y, 14, 14, 2, color);
}


void TFTScreen::draw_pot_icon(uint8_t x, uint8_t y, float percent_of_max){
    const uint8_t radius = 8;
    
    //clear previous pot icon by covering it with background colour circle slightly larger than original
    tft.fillCircle(x, y, radius + 1, bg_color);
    tft.drawCircle(x, y, radius, pot_color);

    //convert percent of max into angle that will work with the trig (limit to range 40-320 deg, reverse the value)
    float angleval = 320.0 - (percent_of_max * 280.0);

    //calculate the line end point with some trig
    int endx = x + int(radius * sin(radians(angleval)));
    int endy = y + int(radius * cos(radians(angleval)));

    tft.drawLine(x,y, endx, endy, pot_color);
}