#ifndef __TFTSCREENBITS_H__
#define __TFTSCREENBITS_H__

#include <Arduino.h>
#include "ILI9341_t3.h"

//Code for controlling the 320x240 TFT screen

//Pin setup chosen based on this page: https://www.pjrc.com/store/display_ili9341.html
#define TFT_DC      20
#define TFT_CS      21
#define TFT_RST     255 //255 as reset pin is not used (connect to 3.3v)
#define TFT_MOSI    7
#define TFT_SCLK    14
#define TFT_MISO    12

class TFTScreen{
    public:
        TFTScreen();
        void clear();
        void print_text(const char* text, uint8_t x, uint8_t y);
        void cover_letter_with_rectangle(uint8_t x, uint8_t y);
        void cover_line_end(uint8_t x, uint8_t y);
        void update();
        void draw_title(const char* text);
        uint16_t get_start_x_for_centred_text(const char* text);

        void draw_colored_pad(uint8_t x, uint8_t y, uint16_t color);
        void draw_pot_icon(uint8_t x, uint8_t y, float percent_of_max);

    private:
        static ILI9341_t3 tft;
        uint16_t bg_color = ILI9341_BLACK;
        uint16_t text_color = ILI9341_GREEN;
        uint16_t pot_color = ILI9341_WHITE;
};

#endif