#ifndef MAX7219_H
#define MAX7219_H

#include <stdint.h>

#define F_CPU 16000000UL

// Address of the register for matrix configuration
#define MAX7219_REG_NOOP 0x00
#define MAX7219_REG_DIGIT0 0x01
#define MAX7219_REG_DIGIT1 0x02
#define MAX7219_REG_DIGIT2 0x03
#define MAX7219_REG_DIGIT3 0x04
#define MAX7219_REG_DIGIT4 0x05
#define MAX7219_REG_DIGIT5 0x06
#define MAX7219_REG_DIGIT6 0x07
#define MAX7219_REG_DIGIT7 0x08
#define MAX7219_REG_DECODEMODE 0x09
#define MAX7219_REG_INTENSITY 0x0A
#define MAX7219_REG_SCANLIMIT 0x0B
#define MAX7219_REG_SHUTDOWN 0x0C
#define MAX7219_REG_DISPLAYTEST 0x0F

#define devicesNum 4

void SPI_init();
void MAX7219_init();
void MAX7219_display(char str[32]);
void MAX7219_displayTest(uint8_t displayTestValue);
void MAX7219_intensity(uint8_t intensityValue);

#endif