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









  
