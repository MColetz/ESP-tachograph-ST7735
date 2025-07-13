# ESP Tachograph with ST7735 Display

A GPS-based tachograph system built with ESP8266, featuring a 128x160 ST7735 TFT display and GPS module for vehicle speed tracking and statistics.

## Features

- **Real-time Speed Display**: Shows current speed in km/h with large, readable digits
- **GPS Integration**: Uses TinyGPS library for accurate location and speed data
- **Multi-tab Interface**: Three main screens - Home, Stats, and Chrono
- **Satellite Status**: Displays number of GPS satellites connected
- **Maximum Speed Tracking**: Records and displays the highest speed achieved
- **Compact Design**: 128x160 pixel display with efficient UI layout

## Hardware Requirements

| Component | Model/Type | Quantity |
|-----------|------------|----------|
| Microcontroller | ESP8266 Development Board | 1 |
| Display | ST7735 128x128 TFT LCD | 1 |
| GPS Module | NEO-6M or compatible | 1 |

## Wiring Tables

### ESP8266 Pin Connections

| ESP8266 Pin | Function | Connected To | Description |
|-----------|----------|--------------|-------------|
| 3.3V | Power | GPS VCC | GPS module power supply |
| GND | Ground | GPS GND, Display GND | Common ground |
| GPIO 0 | Software Serial RX | GPS TX | GPS data receive |
| GPIO 2 | Software Serial TX | GPS RX | GPS data transmit |
| GPIO 4 | Display Reset | Display RST | Display reset signal |
| GPIO 5 | Display CS | Display CS | Display chip select |
| GPIO 13 | Display MOSI | Display SDA | Display data line |
| GPIO 14 | Display SCLK | Display SCK | Display clock line |
| GPIO 16 | Display DC | Display A0/DC | Display data/command |

### Display (ST7735) Pinout

| Display Pin | ESP8266 Pin | Function | Description |
|-------------|-----------|----------|-------------|
| VCC | 3.3V | Power | Display power supply |
| GND | GND | Ground | Common ground |
| SCL | GPIO 14 | SPI Clock | Serial clock signal |
| SDA | GPIO 13 | SPI Data | Serial data line |
| RES | GPIO 4 | Reset | Display reset signal |
| DC | GPIO 16 | Data/Command | Data/command selection |
| CS | GPIO 5 | Chip Select | Chip select signal |
| BLK | 3.3V | Backlight | Display backlight (optional) |

### GPS Module Pinout

| GPS Pin | ESP8266 Pin | Function | Description |
|---------|-----------|----------|-------------|
| VCC | 3.3V | Power | GPS module power |
| GND | GND | Ground | Common ground |
| TX | GPIO 0 | Data Out | GPS data transmission |
| RX | GPIO 2 | Data In | GPS data reception |

## Software Dependencies

| Library | Version | Purpose |
|---------|---------|---------|
| Adafruit_ST7735 | Latest | ST7735 display driver |
| TinyGPS | Latest | GPS data parsing |
| SoftwareSerial | Built-in | Software UART for GPS |

## Installation

1. **Install Required Libraries**:
   - Open Arduino IDE
   - Go to Tools → Manage Libraries
   - Search and install:
     - "Adafruit ST7735 and ST7789 Library"
     - "TinyGPS"

2. **Hardware Setup**:
   - Connect components according to the wiring tables above
   - Ensure proper power supply (3.3V for ESP8266)
   - Double-check all connections before powering on

3. **Upload Code**:
   - Open `ESP-tachograph-ST7735.ino` in Arduino IDE
   - Select your ESP8266 board from Tools → Board
   - Select the correct COM port
   - Click Upload
