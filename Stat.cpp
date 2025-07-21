#include "ESP-tachograph-ST7735.h"
#include "SmartLcdPrint.h"
#include <math.h>

Stat::Stat(Adafruit_ST7735* lcd, TinyGPS* gps) : Tab(lcd, gps) {
    // Initialize display elements
    mean_speed_readout = new LcdFormattedReadout(lcd, 10, 20, 2, 1, 4, "Avg:", " km/h", NAN);
    total_distance_readout = new LcdFormattedReadout(lcd, 10, 45, 2, 1, 5, "Dist:", " km", NAN);
    total_time_readout = new LcdFormattedReadout(lcd, 10, 70, 2, 0, 6, "Time:", "", NAN);
    current_speed_readout = new LcdFormattedReadout(lcd, 10, 95, 2, 1, 4, "Now:", " km/h", NAN);
    
    // Initialize statistics variables
    reset_stats();
}

Stat::~Stat() {
    delete mean_speed_readout;
    delete total_distance_readout;
    delete total_time_readout;
    delete current_speed_readout;
}

void Stat::set_active() {
    _set_footer_active();
    
    // Draw title
    lcd->setTextSize(2);
    lcd->setTextColor(ST77XX_WHITE);
    lcd->setCursor(25, 2);
    lcd->print("STATS");
    
    // Initialize displays with current values
    update_stats();
    mean_speed_readout->print_full_string(sum_speeds / (speed_readings > 0 ? speed_readings : 1));
    total_distance_readout->print_full_string(total_distance);
    
    char time_buffer[10];
    format_time(total_time / 1000, time_buffer);
    total_time_readout->set_suffix(time_buffer);
    total_time_readout->print_full_string(0);
    
    current_speed_readout->print_full_string(gps->f_speed_kmph());
}

void Stat::refresh() {
    update_stats();
    
    // Update displays
    float mean_speed = (speed_readings > 0) ? sum_speeds / speed_readings : 0;
    mean_speed_readout->refresh(mean_speed);
    total_distance_readout->refresh(total_distance);
    
    char time_buffer[10];
    format_time(total_time / 1000, time_buffer);
    total_time_readout->set_suffix(time_buffer);
    total_time_readout->update_value_only(0);
    
    current_speed_readout->refresh(gps->f_speed_kmph());
}

void Stat::reset_stats() {
    start_time = millis();
    total_time = 0;
    total_distance = 0;
    sum_speeds = 0;
    speed_readings = 0;
    last_lat = 0;
    last_lon = 0;
    first_position = true;
}

void Stat::update_stats() {
    unsigned long current_time = millis();
    total_time = current_time - start_time;
    
    float current_speed = gps->f_speed_kmph();
    if (current_speed != TinyGPS::GPS_INVALID_F_SPEED && current_speed > 0) {
        sum_speeds += current_speed;
        speed_readings++;
    }
    
    // Calculate distance traveled using GPS positions
    float current_lat, current_lon;
    unsigned long age;
    gps->f_get_position(&current_lat, &current_lon, &age);
    
    if (age != TinyGPS::GPS_INVALID_AGE && 
        current_lat != TinyGPS::GPS_INVALID_F_ANGLE && 
        current_lon != TinyGPS::GPS_INVALID_F_ANGLE) {
        
        if (!first_position) {
            // Calculate distance between last position and current position
            float lat1_rad = last_lat * PI / 180.0;
            float lon1_rad = last_lon * PI / 180.0;
            float lat2_rad = current_lat * PI / 180.0;
            float lon2_rad = current_lon * PI / 180.0;
            
            float dlat = lat2_rad - lat1_rad;
            float dlon = lon2_rad - lon1_rad;
            float a = sin(dlat/2) * sin(dlat/2) + cos(lat1_rad) * cos(lat2_rad) * sin(dlon/2) * sin(dlon/2);
            float c = 2 * atan2(sqrt(a), sqrt(1-a));
            float distance = 6371 * c; // Earth radius in km
            
            total_distance += distance;
        }
        
        last_lat = current_lat;
        last_lon = current_lon;
        first_position = false;
    }
}

void Stat::format_time(unsigned long seconds, char* buffer) {
    unsigned long hours = seconds / 3600;
    unsigned long minutes = (seconds % 3600) / 60;
    unsigned long secs = seconds % 60;
    
    if (hours > 0) {
        sprintf(buffer, " %lu:%02lu:%02lu", hours, minutes, secs);
    } else {
        sprintf(buffer, " %lu:%02lu", minutes, secs);
    }
}

void Stat::_set_footer_active() {
    //FOOTER active
    lcd->setTextSize(1);
    lcd->fillRect(0, 116, 42, 12, ST77XX_WHITE);
    lcd->setTextColor(ST77XX_BLACK);
    lcd->setCursor(6, 118);
    lcd->print("STATS");
}

void Stat::set_inactive() {
    lcd->setTextSize(1);
    lcd->fillRect(0, 116, 42, 12, ST77XX_BLACK);
    lcd->drawRect(0, 116, 42, 12, ST77XX_WHITE);
    lcd->setTextColor(ST77XX_WHITE);
    lcd->setCursor(6, 118);
    lcd->print("STATS");
} 