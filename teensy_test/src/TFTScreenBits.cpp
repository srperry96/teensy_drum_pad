#include <TFTScreenBits.h>
#include "SharedBits.h"

ILI9341_t3 TFTScreen::tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);
TFTScreen screen;


TFTScreen::TFTScreen(){
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);

    Serial.println("Setup Screen"); 
}

void TFTScreen::clear(){
    tft.fillScreen(ILI9341_BLACK);
}

void TFTScreen::print_text(const char* text, uint8_t x, uint8_t y){
    tft.setCursor(x,y);
    tft.println(text);
}

void TFTScreen::cover_letter_with_rectangle(uint8_t x, uint8_t y){
    tft.fillRect(x, y, 15, 18, ILI9341_BLACK);
}

void TFTScreen::cover_line_end(uint8_t x, uint8_t y){
    tft.fillRect(x, y, 200, 18, ILI9341_BLACK);
}