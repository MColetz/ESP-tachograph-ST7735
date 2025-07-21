#include "SmartLcdPrint.h"

#define INVALID_CHAR '#'

LcdReadout::LcdReadout(Adafruit_ST7735* lcd, int x, int y, int text_size, int precision, float start_value, int width, int height, float invalid_value)
  : lcd(lcd), x(x), y(y), text_size(text_size), precision(precision), last_value(start_value), width(width), height(height), default_color(ST77XX_WHITE), invalid_value(invalid_value) {
    if (!isnan(start_value)) {
      print_value(start_value);
    }
}

void LcdReadout::refresh(float new_value) {
  if (new_value != last_value) {
    print_value(last_value, ST77XX_BLACK);
    print_value(new_value, default_color);
    last_value = new_value;
  }
}

void LcdReadout::set_default_color(uint16_t color) {
  default_color = color;
}

void LcdReadout::print_value(float value) {
  print_value(value, default_color);
}

void LcdReadout::print_value(float value, uint16_t color) {
  lcd->setTextSize(text_size);
  lcd->setTextColor(color);
  lcd->setCursor(x, y);
  if (value == invalid_value) {
    for (int i = 0; i < width / (6 * text_size); i++) { // Fill width with INVALID_CHAR
      lcd->print(INVALID_CHAR);
    }
  } else {
    lcd->print(value, precision);
  }
}

// LcdFormattedReadout implementation
LcdFormattedReadout::LcdFormattedReadout(Adafruit_ST7735* lcd, int x, int y, int text_size, int precision, int max_digits, const char* prefix, const char* suffix, float start_value, float invalid_value)
  : lcd(lcd), x(x), y(y), text_size(text_size), precision(precision), last_value(start_value), default_color(ST77XX_WHITE), max_digits(max_digits), invalid_value(invalid_value) {
  
  // Allocate memory for prefix and suffix
  this->prefix = new char[strlen(prefix) + 1];
  this->suffix = new char[strlen(suffix) + 1];
  strcpy(this->prefix, prefix);
  strcpy(this->suffix, suffix);
  
  calculate_layout();
  
  if (!isnan(start_value)) {
    print_full_string(start_value);
  }
}

LcdFormattedReadout::~LcdFormattedReadout() {
  delete[] prefix;
  delete[] suffix;
}

void LcdFormattedReadout::refresh(float new_value) {
  if (new_value != last_value) {
    update_value_only(new_value);
    last_value = new_value;
  }
}

void LcdFormattedReadout::set_default_color(uint16_t color) {
  default_color = color;
}

void LcdFormattedReadout::set_prefix(const char* new_prefix) {
  delete[] prefix;
  prefix = new char[strlen(new_prefix) + 1];
  strcpy(prefix, new_prefix);
  calculate_layout();
}

void LcdFormattedReadout::set_suffix(const char* new_suffix) {
  delete[] suffix;
  suffix = new char[strlen(new_suffix) + 1];
  strcpy(suffix, new_suffix);
  calculate_layout();
}

void LcdFormattedReadout::print_full_string(float value) {
  print_full_string(value, default_color);
}

void LcdFormattedReadout::print_full_string(float value, uint16_t color) {
  lcd->setTextSize(text_size);
  lcd->setTextColor(color);
  lcd->setCursor(x, y);
  
  // Print prefix
  lcd->print(prefix);
  
  // Print value or INVALID_CHAR if invalid
  if (value == invalid_value) {
    for (int i = 0; i < max_digits; i++) {
      lcd->print(INVALID_CHAR);
    }
  } else {
    lcd->print(value, precision);
  }
  
  // Print suffix at fixed position
  lcd->setCursor(suffix_start_x, y);
  lcd->print(suffix);
}

void LcdFormattedReadout::update_value_only(float value) {
  update_value_only(value, default_color);
}

void LcdFormattedReadout::update_value_only(float value, uint16_t color) {
  // Overwrite the old value in black
  lcd->setTextSize(text_size);
  lcd->setTextColor(ST77XX_BLACK);
  lcd->setCursor(value_start_x, y);
  if (last_value == invalid_value) {
    for (int i = 0; i < max_digits; i++) {
      lcd->print(INVALID_CHAR);
    }
  } else {
    lcd->print(last_value, precision);
  }
  
  // Print new value in the specified color
  lcd->setTextColor(color);
  lcd->setCursor(value_start_x, y);
  if (value == invalid_value) {
    for (int i = 0; i < max_digits; i++) {
      lcd->print(INVALID_CHAR);
    }
  } else {
    lcd->print(value, precision);
  }
}

void LcdFormattedReadout::calculate_layout() {
  // Calculate character dimensions
  int char_width = 6 * text_size;
  int char_height = 8 * text_size;
  
  // Calculate prefix width
  int prefix_width = strlen(prefix) * char_width;
  
  // Calculate fixed value width based on max_digits
  int value_width = max_digits * char_width;
  
  // Calculate suffix width
  int suffix_width = strlen(suffix) * char_width;
  
  // Calculate total dimensions
  total_width = prefix_width + value_width + suffix_width;
  total_height = char_height;
  
  // Calculate fixed positions
  value_start_x = x + prefix_width;
  suffix_start_x = value_start_x + value_width;
}

int LcdFormattedReadout::calculate_digits_needed(float value) {
  if (isnan(value)) return 3; // "NaN"
  
  int digits = 1; // At least one digit
  
  // Count integer digits
  int int_part = (int)abs(value);
  while (int_part >= 10) {
    digits++;
    int_part /= 10;
  }
  
  // Add decimal point and precision if needed
  if (precision > 0) {
    digits += 1 + precision; // +1 for decimal point
  }
  
  // Add sign if negative
  if (value < 0) digits++;
  
  return digits;
}


