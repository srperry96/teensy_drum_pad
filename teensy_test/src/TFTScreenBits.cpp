#include <TFTScreenBits.h>

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_MISO);

void setup_screen(){
    tft.begin();
    //rotation will be 1 or 3, depending on which way the screen is eventually mounted
    tft.setRotation(3);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextColor(ILI9341_YELLOW);
    tft.setTextSize(2);
    tft.println("Big Beatz Machine");
    tft.println("------------------");
}

//OLD TEST FUNCTION - LEAVING HERE AS REFERENCE FOR LATER ON
// void print_filepath(const char* path){
//  //fill background with black (clear screen)
//   tft.fillScreen(ILI9341_BLACK);
//  //set cursor to top left of screen again
//   tft.setCursor(0,0);
//  //print some text
//   tft.println(path);
// }