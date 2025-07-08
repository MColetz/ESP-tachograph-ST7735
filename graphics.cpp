#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 16
#define TFT_SCLK 14
#define TFT_MOSI 13

static auto tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup_graphics() {
  Serial.println("Initializing TFT SCREEN");
  tft.initR(INITR_BLACKTAB); // Init ST7735S chip, black tab
  tft.fillScreen(ST77XX_BLACK);
  Print_logo();
}

void Screen_SM(){
  if(screen_current_state == HOME)
  {
    display_home();
  }
  else if(screen_current_state == STATS)
  {
    tft.setCursor(0, 0);
    tft.println("STATS");
    // Aggiungi altri dati utili
  }
  else if(screen_current_state == CHRONO)
  {
    tft.setCursor(0, 0);
    tft.println("CHRONO");
    // Logica per il cronometro
  }
}