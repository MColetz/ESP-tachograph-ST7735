#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <array>
#include <TinyGPS.h>

class Tab {
  protected:
    Adafruit_ST7735* lcd;
    TinyGPS* gps;
    
  public:
    Tab(Adafruit_ST7735* lcd, TinyGPS* gps) : lcd(lcd), gps(gps) {
    }
    virtual void refresh() = 0;
    virtual void set_active() = 0;
    virtual void set_inactive() = 0;
};

class Home : public Tab {
	public:
	Home(Adafruit_ST7735* lcd, TinyGPS* gps) : Tab(lcd, gps) {
	}
    
    void set_active() {
        
        _set_footer_active();
        
        // === VELOCITÀ ATTUALE CENTRALE ===
        float current_speed = gps->f_speed_kmph();
        
        lcd->setTextSize(3);
        lcd->setCursor(24, 37);
        lcd->setTextColor(ST77XX_WHITE);
        lcd->print(current_speed, 1);
        
        lcd->setTextSize(1);
        lcd->setCursor(90, 60);
        lcd->print("km/h");
    }

    void refresh() override {
        
    }
    
    void _set_footer_active() {
        //FOOTER active
        lcd->setTextSize(1);
        lcd->fillRect(43, 116, 42, 12, ST77XX_WHITE);
        lcd->setTextColor(ST77XX_BLACK);
        lcd->setCursor(52, 118);
        lcd->print("HOME");
    }
    
    void set_inactive() {
        lcd->setTextSize(1);
        lcd->fillRect(43, 116, 42, 12, ST77XX_BLACK);
        lcd->drawRect(43, 116, 42, 12, ST77XX_WHITE);
        lcd->setTextColor(ST77XX_WHITE);
        lcd->setCursor(52, 118);
        lcd->print("HOME");
    }
};

class Stat : public Tab {
	public:
	Stat(Adafruit_ST7735* lcd, TinyGPS* gps) : Tab(lcd, gps) {
            // metti qui tutti gli attributi di Stat
	}

  void set_active() {;
      _set_footer_active();
  }

	void refresh() override {
		// qui fai il refresh della Stat
	}    

  void _set_footer_active() {
      //FOOTER active
      lcd->setTextSize(1);
      lcd->fillRect(0, 116, 42, 12, ST77XX_WHITE);
      lcd->setTextColor(ST77XX_BLACK);
      lcd->setCursor(6, 118);
      lcd->print("STATS");
  }

  void set_inactive() {
      lcd->setTextSize(1);
      lcd->fillRect(0, 116, 42, 12, ST77XX_BLACK);
      lcd->drawRect(0, 116, 42, 12, ST77XX_WHITE);
      lcd->setTextColor(ST77XX_WHITE);
      lcd->setCursor(6, 118);
      lcd->print("STATS");
  }
}; 

class Chrono : public Tab {
	public:
	Chrono(Adafruit_ST7735* lcd, TinyGPS* gps) : Tab(lcd, gps) {
            // metti qui tutti gli attributi di Chrono
	}

    void set_active() {
        _set_footer_active();

    }

	void refresh() override {
            // qui fai il refresh della Chrono
	}


  void _set_footer_active() {
      //FOOTER active
      lcd->setTextSize(1);
      lcd->fillRect(86, 116, 42, 12, ST77XX_WHITE);
      lcd->setTextColor(ST77XX_BLACK);
      lcd->setCursor(92, 118);
      lcd->print("CRONO");
  }

  void set_inactive() {
      lcd->setTextSize(1);
      lcd->fillRect(86, 116, 42, 12, ST77XX_BLACK);
      lcd->drawRect(86, 116, 42, 12, ST77XX_WHITE);
      lcd->setTextColor(ST77XX_WHITE);
      lcd->setCursor(92, 118);
      lcd->print("CRONO");
  }
}; 


class Display {
	Tab* active_tab;
  int current_tab_idx;
	std::array<Tab*, 3> tabs;
  float maxgps_speed;
  Adafruit_ST7735* lcd;
  TinyGPS* gps;
	public:
  Display() {};
	Display(std::array<Tab*, 3> my_tabs, Adafruit_ST7735* lcd, TinyGPS* gps) : tabs(my_tabs), lcd(lcd), gps(gps) {
    active_tab = my_tabs[1];
    current_tab_idx = 1;	
    maxgps_speed = 0;
    //TOP INIT
    _init_top_bar();

    // FOOTER INIT
    lcd->fillRect(0, 116, 128, 12, ST77XX_BLACK);
    for(auto tab:tabs){
      tab->set_inactive();
    }
    // footer init
	}

	void changeState(int idx) {
    //EMPTY SCREEN
    lcd->fillRect(0, 0, 124, 115, ST77XX_BLACK);

    active_tab->set_inactive();
    active_tab = tabs[idx];
    current_tab_idx = idx;
    active_tab->set_active();        
    active_tab->refresh();	
  }

  void update_data(){
    _update_top_bar();
    active_tab->refresh();
  }

  void _init_top_bar(){
    //HOME LAYOUT
    lcd->fillRect(0, 0, 128, 12, ST77XX_BLACK);
    lcd->drawRect(0, 0, 64, 12, ST77XX_WHITE);
    lcd->drawRect(64, 0, 64, 12, ST77XX_WHITE);


    lcd->setTextColor(ST77XX_WHITE);
    lcd->setTextSize(1);

    lcd->setCursor(2, 2);
    lcd->print("Sats: ");  //X in [2;38] Y in [2;10]
    if(gps->satellites() == 255 || gps->satellites() == -1){
      lcd->print("##");
    }else{
      lcd->print(gps->satellites());//X in [38;56] (3 digit) Y in [2;10]
    }
    
    lcd->setCursor(66, 2);
    lcd->print("Max: "); //X in [66; 96] Y in [2;10]
    lcd->print(maxgps_speed, 1); //X in [96;120] (4 digit 24 px) Y in [2;10]
  }

  void _update_top_bar(){
    lcd->fillRect(38, 2, 20, 8, ST77XX_BLACK);
    lcd->fillRect(96, 2, 26, 8, ST77XX_BLACK);

    lcd->setTextColor(ST77XX_WHITE);
    lcd->setTextSize(1);

    if (gps->f_speed_kmph() > maxgps_speed) maxgps_speed = gps->f_speed_kmph();

    lcd->setCursor(38, 2);
    if(gps->satellites() == 255 || gps->satellites() == -1){
      lcd->print("##");
    }else{
      lcd->print(gps->satellites());//X in [38;56] (3 digit) Y in [2;10]
    }

    lcd->setCursor(96, 2);
    lcd->print(maxgps_speed, 1); //X in [96;120] (4 digit 24 px) Y in [2;10]

  }
};


 