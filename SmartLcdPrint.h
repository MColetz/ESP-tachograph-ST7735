#pragma once

#include <Adafruit_ST7735.h>
#include <math.h>
#include <string.h>

class LcdReadout {
    private:
      Adafruit_ST7735* lcd;
      int x, y;
      int text_size;
      int precision; 
      float last_value;
      int width, height;
      uint16_t default_color;
  
    public:
      LcdReadout(Adafruit_ST7735* lcd, int x, int y, int text_size, int precision, float start_value = NAN, int width = 40, int height = 20);
      void refresh(float new_value);
      void set_default_color(uint16_t color);
      void print_value(float value); // Uses default_color
      void print_value(float value, uint16_t color);
  
    protected:
      Adafruit_ST7735* get_lcd() { return lcd; }
      int get_x() { return x; }
      int get_y() { return y; }
      int get_width() { return width; }
      int get_height() { return height; }
};

class LcdBoxedReadout : public LcdReadout {
    private:
      uint16_t box_color;
      int border_width;
  
    public:
      LcdBoxedReadout(Adafruit_ST7735* lcd, int x, int y, int text_size, int precision, float start_value = NAN, int width = 40, int height = 20, int border_width = 4, uint16_t box_color = ST77XX_WHITE);
      void set_box_color(uint16_t color);
      void draw_box();
};

class LcdFormattedReadout {
    private:
      Adafruit_ST7735* lcd;
      int x, y;
      int text_size;
      int precision;
      float last_value;
      uint16_t default_color;
      char* prefix;
      char* suffix;
      int max_digits;
      int value_start_x;
      int suffix_start_x;
      int total_width;
      int total_height;
  
    public:
      LcdFormattedReadout(Adafruit_ST7735* lcd, int x, int y, int text_size, int precision, int max_digits, const char* prefix = "", const char* suffix = "", float start_value = NAN);
      ~LcdFormattedReadout();
      void refresh(float new_value);
      void set_default_color(uint16_t color);
      void set_prefix(const char* new_prefix);
      void set_suffix(const char* new_suffix);
      void print_full_string(float value);
      void print_full_string(float value, uint16_t color);
      void update_value_only(float value);
      void update_value_only(float value, uint16_t color);
  
    private:
      void calculate_layout();
      int calculate_digits_needed(float value);
}; 