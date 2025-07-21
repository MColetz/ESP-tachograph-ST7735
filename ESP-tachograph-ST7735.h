#pragma once

#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <array>
#include <TinyGPS.h>
#include "bitmaps.h"
#include "SmartLcdPrint.h"

// Forward declarations
class Tab;
class Home;
class Stat;
class Chrono;
class Display;
class LcdReadout;
class LcdFormattedReadout;

class Tab {
  protected:
    Adafruit_ST7735* lcd;
    TinyGPS* gps;
    
  public:
    Tab(Adafruit_ST7735* lcd, TinyGPS* gps);
    virtual void refresh() = 0;
    virtual void set_active() = 0;
    virtual void set_inactive() = 0;
};

class Home : public Tab {
	public:
	Home(Adafruit_ST7735* lcd, TinyGPS* gps);
    void set_active();
    void refresh() override;
    void set_inactive();
    
  private:
    void _set_footer_active();
    LcdReadout* speed_readout;
    LcdFormattedReadout* altitude_readout;
    LcdFormattedReadout* course_readout;
    LcdLabel* kmh_label; // Add member variable
};

class Stat : public Tab {
	public:
	Stat(Adafruit_ST7735* lcd, TinyGPS* gps);
	~Stat();
    void set_active();
	void refresh() override;
    void set_inactive();
    void reset_stats();
    void update_stats();  // Made public for background updates
    
  private:
    void _set_footer_active();
    void format_time(unsigned long seconds, char* buffer);
    
    // Statistics tracking variables
    unsigned long start_time;
    unsigned long total_time;
    float total_distance;
    float sum_speeds;
    unsigned long speed_readings;
    float last_lat, last_lon;
    bool first_position;
    
    // Display elements
    LcdFormattedReadout* mean_speed_readout;
    LcdFormattedReadout* total_distance_readout;
    LcdFormattedReadout* total_time_readout;
    LcdFormattedReadout* current_speed_readout;
}; 

class Chrono : public Tab {
	public:
	Chrono(Adafruit_ST7735* lcd, TinyGPS* gps);
	~Chrono();
    void set_active();
	void refresh() override;
    void set_inactive();
    
  private:
    void _set_footer_active();
    float calculate_distance_to_home();
    float calculate_bearing_to_home();
    float calculate_eta_to_home();
    
    // Home position coordinates (fixed in code)
    const float HOME_LAT = 41.9028;  // Rome, Italy - CHANGE TO YOUR HOME LOCATION
    const float HOME_LON = 12.4964;  // Rome, Italy - CHANGE TO YOUR HOME LOCATION
    
    // Display elements
    LcdFormattedReadout* distance_readout;
    LcdFormattedReadout* bearing_readout;
    LcdFormattedReadout* eta_readout;
}; 

class Display {
	Tab* active_tab;
    int current_tab_idx;
	std::array<Tab*, 3> tabs;
    float maxgps_speed;
    Adafruit_ST7735* lcd;
    TinyGPS* gps;
    LcdFormattedReadout* satellites_readout;
    LcdFormattedReadout* max_speed_readout;
    
	public:
    Display();
	Display(std::array<Tab*, 3> my_tabs, Adafruit_ST7735* lcd, TinyGPS* gps);
	~Display();
	void changeState(int idx);
    void update_data();
    
  private:
    void _init_top_bar();
    void _update_top_bar();
};


