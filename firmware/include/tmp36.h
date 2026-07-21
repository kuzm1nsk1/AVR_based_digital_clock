#ifndef TMP36_H
#define TMP36_H

#include <avr/io.h>

void adc_init();
uint16_t adc_read(uint8_t channel);
float read_temperature();

#endif