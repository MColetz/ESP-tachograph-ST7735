#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <array>
#include <TinyGPS.h>


class Tab {
  protected:
    Adafruit_ST7735* lcd;
    TinyGPS* gps;
    float maxgps_speed;
  public:
    Tab(Adafruit_ST7735* lcd, TinyGPS* gps) : lcd(lcd), gps(gps) {
        maxgps_speed = 0;
    }
    virtual void refresh() = 0;
    virtual void init_footer() = 0;
    virtual void set_active() = 0;
    virtual void set_inactive() = 0;
};

class Home : public Tab {
	public:
	Home(Adafruit_ST7735* lcd, TinyGPS* gps) : Tab(lcd, gps) {
	}

	void refresh() override {
        
	}
    
    void init_footer() {
        lcd->drawRect(43, 116, 42, 12, ST77XX_WHITE);
        lcd->setTextColor(ST77XX_WHITE);
        lcd->setCursor(52, 118);
        lcd->print("HOME");
    }

    void set_active() {
        //EMPTY SCREEN
        lcd->fillRect(0, 0, 124, 115, ST77XX_BLACK);

        //HOME LAYOUT
        lcd->setTextColor(ST77XX_WHITE);
        lcd->setTextSize(1);
        lcd->setCursor(2, 14);
        lcd->print("Sats: ");
        lcd->print(gps->satellites());
        
        lcd->setCursor(70, 14);
        lcd->print("Max: ");
        lcd->print(maxgps_speed, 1);
        lcd->print("k");

        // === VELOCITÀ ATTUALE CENTRALE ===
        float current_speed = gps->f_speed_kmph();
        if (current_speed > maxgps_speed) maxgps_speed = current_speed;

        lcd->setTextSize(3);
        lcd->setCursor(20, 40);
        lcd->setTextColor(ST77XX_WHITE);
        lcd->print(current_speed, 1);

        lcd->setTextSize(1);
        lcd->setCursor(90, 60);
        lcd->print("km/h");  // qui fai il refresh della Home

        //FOOTER active
        lcd->fillRect(43, 116, 42, 12, ST77XX_WHITE);
        lcd->setTextColor(ST77XX_BLACK);
        lcd->setCursor(52, 118);
        lcd->print("HOME");
    }

    void set_inactive() {
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

	void refresh() override {
		// qui fai il refresh della Stat
	}

    void init_footer() {
        // qui fai il print del footer di Stat
        lcd->setTextColor(ST77XX_WHITE);
        lcd->drawRect(0, 116, 42, 12, ST77XX_WHITE);
        lcd->setCursor(6, 118);
        lcd->print("STATS");
    }

    void set_active() {
        lcd->fillRect(0, 116, 42, 12, ST77XX_WHITE);
        lcd->setTextColor(ST77XX_BLACK);
        lcd->setCursor(6, 118);
        lcd->print("STATS");
    }

    void set_inactive() {
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

	void refresh() override {
            // qui fai il refresh della Chrono
	}

    void init_footer() {
        // qui fai il print del footer di Crhono
        lcd->setTextColor(ST77XX_WHITE);
        lcd->drawRect(86, 116, 42, 12, ST77XX_WHITE);
        lcd->setCursor(92, 118);
        lcd->print("CRONO");
    }

    void set_active() {
        lcd->fillRect(86, 116, 42, 12, ST77XX_WHITE);
        lcd->setTextColor(ST77XX_BLACK);
        lcd->setCursor(92, 118);
        lcd->print("CRONO");
    }

    void set_inactive() {
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

	public:
  Display() {};
	Display(std::array<Tab*, 3> my_tabs, Adafruit_ST7735* lcd) : tabs(my_tabs) {

        active_tab = my_tabs[1];
        current_tab_idx = 1;	
        // Barra nera
        lcd->fillRect(0, 116, 128, 12, ST77XX_BLACK);
        for(auto tab:tabs){
          tab->init_footer();
        }
        // footer init

	}

	void changeState(int idx) {
        active_tab->set_inactive();
        active_tab = tabs[idx];
        current_tab_idx = idx;
        active_tab->set_active();        
        active_tab->refresh();	
    }

  void update_data(){
    active_tab->refresh();
  }
};


 