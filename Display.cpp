#include "ESP-tachograph-ST7735.h"
#include "SmartLcdPrint.h"

Display::Display() {
}

Display::Display(std::array<Tab*, 3> my_tabs, Adafruit_ST7735* lcd, TinyGPS* gps) : tabs(my_tabs), lcd(lcd), gps(gps) {
    current_tab_idx = 1;	
    active_tab = tabs[current_tab_idx];
    maxgps_speed = 0;

    satellites_readout = new LcdFormattedReadout(lcd, 5, 2, 1, 0, 2, "Sats:", "", NAN);
    max_speed_readout = new LcdFormattedReadout(lcd, 68, 2, 1, 1, 4, "Max:", "", NAN);
    
    //TOP INIT
    delay(200);
    lcd->drawBitmap(0, 12, piaggio_ciao_bitmap, 128, 100, ST77XX_WHITE);
    delay(2000);
    
    _init_top_bar();
    // FOOTER INIT
    lcd->fillRect(0, 116, 128, 12, ST77XX_BLACK);
    for(auto tab:tabs){
      tab->set_inactive();
    }
    changeState(1); 
    // footer init
}

Display::~Display() {
    delete satellites_readout;
    delete max_speed_readout;
}

void Display::changeState(int idx) {
    //EMPTY SCREEN
    lcd->fillRect(0, 13, 128, 103, ST77XX_BLACK);

    active_tab->set_inactive();
    
    active_tab = tabs[idx];
    current_tab_idx = idx;
    active_tab->set_active();        
    active_tab->refresh();	
}

void Display::update_data(){
    _update_top_bar();
    
    // Update stats in background regardless of active tab
    if (tabs[0] != nullptr) {  // Stat tab
        static_cast<Stat*>(tabs[0])->update_stats();
    }
    
    // Update active tab display
    active_tab->refresh();
}

void Display::_init_top_bar(){
    //HOME LAYOUT
    lcd->fillRect(0, 0, 128, 12, ST77XX_BLACK);
    lcd->drawRect(0, 0, 64, 12, ST77XX_WHITE);
    lcd->drawRect(63, 0, 65, 12, ST77XX_WHITE);

    // Initialize the readouts with current values
    satellites_readout->print_full_string(0);
    max_speed_readout->print_full_string(0);
}

void Display::_update_top_bar(){
    // Update satellites count
    int satellites = gps->satellites();
    if(satellites == 255 || satellites == -1){
        satellites = 0;
    }
    
    // Update max speed
    if (gps->f_speed_kmph() > maxgps_speed) {
        maxgps_speed = gps->f_speed_kmph();
    }
    
    // Refresh displays
    satellites_readout->refresh(satellites);
    max_speed_readout->refresh(maxgps_speed);
} 