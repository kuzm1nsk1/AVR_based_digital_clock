# AVR_based_digital_clock

Modification of a project: `arduino_GPS_synchronized_clock:` https://github.com/kuzm1nsk1/arduino_GPS_synchronized_clock <br>
It uses standalone ATmega328P microcontroller for processing and DS3231 RTC module for time keeping.

## Features
- 32x8 LED matrix display
- adjustable display brightness
- temperature display
- easy and intuitive time and alarm setting
- speaker to sound the alarm
- settings are storred in case of power failure

## Hardware requirements
- ATmega328P microcontroller
- TMP36 temperature sensor
- MAX7219 LED matrix module
- DS3231 RTC module
- 16 MHz crystal
- 2 22pF capacitors
- 4 100nF capacitors
- 10uF capacitor
- 2 4k7 resistors
- 2 10k resistors
- pushbutton
- DC barrel jack
- rotary encoder
- AVR programmer

## Software requirements
- Visual Studio Code with PlatformIO IDE or Microchip Studio for AVR and SAM Devices

### Required libraries
```
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <util/delay.h> 
#include <avr/eeprom.h>
```
Libraries for MAX7219, DS3231, TMP36 and rotary encoder are located in docs folder

## Wiring and schematics
### Board schematic
![avr_clock](https://github.com/user-attachments/assets/7486519d-efc3-4287-8995-e512410f21d6) <br><br>

## Troubleshooting
### ATmega328P standadlone circuit
![avr_clock_atmega328p](https://github.com/user-attachments/assets/70ec5d85-9575-483d-a802-56d4d92f22aa)

### Arduino Uno used instead of ATmega328P
![avr_clock_arduino](https://github.com/user-attachments/assets/d48ee1dd-e7a8-4457-b35f-92c739b47c62) <br><br>

**Problem:** ATmega328P displaying random characters <br>
**Solution:** Faulty ATmega328P microcontroller, replace it with a new one

## Demonstration video

https://github.com/user-attachments/assets/a4d47e3d-2e19-4f5e-987f-0afac1b2c268














  
