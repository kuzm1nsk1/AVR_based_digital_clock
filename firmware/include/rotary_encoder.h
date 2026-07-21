#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <avr/io.h>
#include <avr/interrupt.h>

void encoder_init(void);
int16_t encoder_read(void);
uint8_t encoder_button_pressed(void);

#endif