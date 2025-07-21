#include "ESP-tachograph-ST7735.h"
#include "SmartLcdPrint.h"
#include <math.h>

Chrono::Chrono(Adafruit_ST7735* lcd, TinyGPS* gps) : Tab(lcd, gps) {
    // Initialize display elements for home navigation
    distance_readout = new LcdFormattedReadout(lcd, 10, 30, 2, 1, 5, "Dist:", " km", NAN);
    bearing_readout = new LcdFormattedReadout(lcd, 10, 60, 2, 0, 3, "Dir:", " °", NAN);
    eta_readout = new LcdFormattedReadout(lcd, 10, 90, 2, 0, 4, "ETA:", " min", NAN);
}

Chrono::~Chrono() {
    delete distance_readout;
    delete bearing_readout;
    delete eta_readout;
}

void Chrono::set_active() {
    _set_footer_active();
    
    // Initialize displays with current values
    float distance = calculate_distance_to_home();
    distance_readout->print_full_string(distance);
    bearing_readout->print_full_string(calculate_bearing_to_home());
    eta_readout->print_full_string(calculate_eta_to_home());
}

void Chrono::refresh() {
    float distance = calculate_distance_to_home();
    float bearing = calculate_bearing_to_home();
    float eta = calculate_eta_to_home();
    
    distance_readout->refresh(distance);
    bearing_readout->refresh(bearing);
    eta_readout->refresh(eta);
}

void Chrono::_set_footer_active() {
    //FOOTER active
    lcd->setTextSize(1);
    lcd->fillRect(86, 116, 42, 12, ST77XX_WHITE);
    lcd->setTextColor(ST77XX_BLACK);
    lcd->setCursor(92, 118);
    lcd->print("CHRONO");
}

void Chrono::set_inactive() {
    lcd->setTextSize(1);
    lcd->fillRect(86, 116, 42, 12, ST77XX_BLACK);
    lcd->drawRect(86, 116, 42, 12, ST77XX_WHITE);
    lcd->setTextColor(ST77XX_WHITE);
    lcd->setCursor(92, 118);
    lcd->print("CHRONO");
}

float Chrono::calculate_distance_to_home() {
    float current_lat, current_lon;
    unsigned long age;
    
    gps->f_get_position(&current_lat, &current_lon, &age);
    
    // Check if GPS data is valid
    if (age == TinyGPS::GPS_INVALID_AGE || 
        current_lat == TinyGPS::GPS_INVALID_F_ANGLE || 
        current_lon == TinyGPS::GPS_INVALID_F_ANGLE) {
        return NAN;
    }
    
    // Convert degrees to radians
    float lat1_rad = current_lat * PI / 180.0;
    float lon1_rad = current_lon * PI / 180.0;
    float lat2_rad = HOME_LAT * PI / 180.0;
    float lon2_rad = HOME_LON * PI / 180.0;
    
    // Haversine formula
    float dlat = lat2_rad - lat1_rad;
    float dlon = lon2_rad - lon1_rad;
    float a = sin(dlat/2) * sin(dlat/2) + cos(lat1_rad) * cos(lat2_rad) * sin(dlon/2) * sin(dlon/2);
    float c = 2 * atan2(sqrt(a), sqrt(1-a));
    float distance = 6371 * c; // Earth radius in km
    
    return distance;
}

float Chrono::calculate_bearing_to_home() {
    float current_lat, current_lon;
    unsigned long age;
    
    gps->f_get_position(&current_lat, &current_lon, &age);
    
    // Check if GPS data is valid
    if (age == TinyGPS::GPS_INVALID_AGE || 
        current_lat == TinyGPS::GPS_INVALID_F_ANGLE || 
        current_lon == TinyGPS::GPS_INVALID_F_ANGLE) {
        return NAN;
    }
    
    // Convert degrees to radians
    float lat1_rad = current_lat * PI / 180.0;
    float lon1_rad = current_lon * PI / 180.0;
    float lat2_rad = HOME_LAT * PI / 180.0;
    float lon2_rad = HOME_LON * PI / 180.0;
    
    float dlon = lon2_rad - lon1_rad;
    float y = sin(dlon) * cos(lat2_rad);
    float x = cos(lat1_rad) * sin(lat2_rad) - sin(lat1_rad) * cos(lat2_rad) * cos(dlon);
    float bearing = atan2(y, x) * 180.0 / PI;
    
    // Convert to 0-360 range
    if (bearing < 0) {
        bearing += 360.0;
    }
    
    return bearing;
}

float Chrono::calculate_eta_to_home() {
    float distance = calculate_distance_to_home();
    float speed = gps->f_speed_kmph();
    
    // Check if data is valid
    if (isnan(distance) || speed == TinyGPS::GPS_INVALID_F_SPEED || speed <= 0) {
        return NAN;
    }
    
    // Calculate ETA in minutes
    float eta_hours = distance / speed;
    float eta_minutes = eta_hours * 60.0;
    
    return eta_minutes;
} 