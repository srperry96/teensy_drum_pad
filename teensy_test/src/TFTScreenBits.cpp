#include <TFTScreenBits.h>
#include "SharedBits.h"

// TFTScreen screen;

TFTScreen::TFTScreen(){
    ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

    tft.begin();
    //rotation will be 1 or 3, depending on which way the screen is eventually mounted
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println("     Big Beatz Machine");
    tft.println("    -------------------");
    tft.println("Hold any pad for menu");
    tft.println();
    tft.println();
    tft.println("Menu to be added...");

    Serial.println("Setup Screen");
}


void TFTScreen::clear(){
    tft.fillScreen(ILI9341_BLACK);
}

void TFTScreen::print_text(const char* text){
    tft.setCursor(0,0);
    tft.println(text);
}