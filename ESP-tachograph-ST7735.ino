#define test

#include "graphics.h"

//DEFINE HARDWARE
TinyGPS gps;
SoftwareSerial ss(0, 2);

//DEFINE VARIABLES
int last_data_age = 0;
double maxgps_speed;
double maxgps_alt;
String heading;
float p = 3.1415926;
enum screen_SM_stats {HOME, STATS, CHRONO};
screen_SM_stats screen_current_state = HOME;
bool printed_title = false;

//DEFINE FUNCTIONS
static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);


void setup(void) {
  Serial.begin(115200);

  setup_graphics();

  Serial.println("Initializing GPS");
  ss.begin(9600);
  Screen_SM();
}

void loop() {
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  gps.f_get_position(&flat, &flon, &age);
  if(age > last_data_age){
    last_data_age = age;
  }else{
    if( gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES){
      Serial.println("Looking for signal");
    }else{
      #ifdef test
      if (printed_title == false){
        #ifdef test
        Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Chars Sentences Checksum");
        Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  RX    RX        Fail");
        Serial.println("----------------------------------------------------------------------------------------------------------------");
        printed_title = true;
        #endif
      }
      print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
      print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
      gps.f_get_position(&flat, &flon, &age);
      print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
      print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
      print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
      print_date(gps);
      print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
      print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
      print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
      print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
      gps.stats(&chars, &sentences, &failed);
      print_int(chars, 0xFFFFFFFF, 6);
      print_int(sentences, 0xFFFFFFFF, 10);
      print_int(failed, 0xFFFFFFFF, 9);
      Serial.println();
      #endif
      //Screen_SM();
      }
  }

  smartdelay(100);


}

void Print_logo(){
  delay(100);
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

void display_home(){
  tft.fillScreen(ST77XX_BLACK);
  
  // === HEADER (barra superiore) ===
  tft.fillRect(0, 0, 128, 12, ST77XX_WHITE); // barra superiore vuota
  
  // === INFO SATELLITI E VELOCITÀ MASSIMA ===
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
  tft.setCursor(2, 14);
  tft.print("Sats: ");
  tft.print(gps.satellites());
  
  tft.setCursor(70, 14);
  tft.print("Max: ");
  tft.print(maxgps_speed, 1);
  tft.print("k");

  // === VELOCITÀ ATTUALE CENTRALE ===
  float current_speed = gps.f_speed_kmph();
  if (current_speed > maxgps_speed) maxgps_speed = current_speed;

  tft.setTextSize(3);
  tft.setCursor(20, 40);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(current_speed, 1);

  tft.setTextSize(1);
  tft.setCursor(90, 60);
  tft.print("km/h");

  // === FOOTER (barra inferiore) ===
  // Barra nera
  tft.fillRect(0, 116, 128, 12, ST77XX_BLACK);

  // STATS
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(0, 116, 42, 12, ST77XX_WHITE);
  tft.setCursor(6, 118);
  tft.print("STATS");

  // HOME (selezionato)
  tft.fillRect(43, 116, 42, 12, ST77XX_WHITE);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(52, 118);
  tft.print("HOME");

  // CRONO
  tft.setTextColor(ST77XX_WHITE);
  tft.drawRect(86, 116, 42, 12, ST77XX_WHITE);
  tft.setCursor(92, 118);
  tft.print("CRONO");
}


#ifdef test
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
#endif
