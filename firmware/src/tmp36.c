#include "tmp36.h"

// Initialize ADC
void adc_init() {
    // Set reference voltage to Avcc
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);
    // Set prescaler to 128
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // Enable ADC
    ADCSRA |= (1 << ADEN);
}

// Read ADC value
uint16_t adc_read(uint8_t channel) {
    // Select ADC channel
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
    // Start conversion
    ADCSRA |= (1 << ADSC);
    // Wait until conversion is complete
    while (ADCSRA & (1 << ADSC));
    // Return ADC value
    return ADC;
}

// Read temperature
float read_temperature() {
    // Read ADC value
    uint16_t adc_value = adc_read(3);
    // Convert ADC value to voltage
    float voltage = adc_value * (5.0 / 1024.0);
    // Convert voltage to temperature
    float temperature = (voltage - 0.5) * 100.0;
    // Return temperature
    return temperature;
}