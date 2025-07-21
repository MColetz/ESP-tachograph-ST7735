#include "ESP-tachograph-ST7735.h"
#include "SmartLcdPrint.h"

Home::Home(Adafruit_ST7735* lcd, TinyGPS* gps) : Tab(lcd, gps) {
    speed_readout = new LcdReadout(lcd, 24, 37, 3, 1, NAN, 60, 24, TinyGPS::GPS_INVALID_F_SPEED); 
    altitude_readout = new LcdFormattedReadout(lcd, 11, 68, 2, 0, 3, "Alt:", " m", NAN, TinyGPS::GPS_INVALID_F_ALTITUDE);
    course_readout = new LcdFormattedReadout(lcd, 11, 88, 2, 0, 3, "Crs:", "", NAN, TinyGPS::GPS_INVALID_F_ANGLE);
    kmh_label = new LcdLabel(lcd, 90, 50, 1, ST77XX_WHITE, "km/h"); // Add label
}

void Home::set_active() {
    _set_footer_active();
    speed_readout->print_value(gps->f_speed_kmph());
    altitude_readout->print_full_string(gps->f_altitude());   
    course_readout->print_full_string(gps->f_course());
    kmh_label->draw(); // Draw the label
}

void Home::refresh() {
    speed_readout->refresh(gps->f_speed_kmph());
    altitude_readout->refresh(gps->f_altitude());
    course_readout->refresh(gps->f_course());
}

void Home::_set_footer_active() {
    //FOOTER active
    lcd->setTextSize(1);
    lcd->fillRect(43, 116, 42, 12, ST77XX_WHITE);
    lcd->setTextColor(ST77XX_BLACK);
    lcd->setCursor(52, 118);
    lcd->print("HOME");
}

void Home::set_inactive() {
    lcd->setTextSize(1);
    lcd->fillRect(43, 116, 42, 12, ST77XX_BLACK);
    lcd->drawRect(43, 116, 42, 12, ST77XX_WHITE);
    lcd->setTextColor(ST77XX_WHITE);
    lcd->setCursor(52, 118);
    lcd->print("HOME");
}