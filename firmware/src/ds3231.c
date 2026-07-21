#include "ds3231.h"
#include <util/twi.h>

// Initialize TWI or SPI interface
void TWI_init(void) {
    TWSR = 0x00; // Prescaler 0
    TWBR = 0x47; // SCL frequency is 50kHz
    TWCR = (1 << TWEN); // Enable TWI
}

// Generate TWI start condition
void TWI_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

// Generate TWI stop condition
void TWI_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

// Write a byte of data to the TWI bus
void TWI_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

// Read a byte of data from the TWI bus and send ACK
uint8_t TWI_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

// Read a byte of data from the TWI bus and send NACK
uint8_t TWI_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

// Convert binary coded decimal (BCD) to decimal
uint8_t bcd_to_dec(uint8_t val) {
    return ((val / 16 * 10) + (val % 16));
}

// Convert decimal to binary coded decimal (BCD)
uint8_t dec_to_bcd(uint8_t val) {
    return ((val / 10 * 16) + (val % 10));
}

// Initialize DS3231 RTC
void DS3231_init(void) {
    TWI_init();
}

// Set time on the RTC
void DS3231_setTime(uint8_t hour, uint8_t min, uint8_t sec) {
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_WRITE);
    TWI_write(0x00);
    TWI_write(dec_to_bcd(sec));
    TWI_write(dec_to_bcd(min));
    TWI_write(dec_to_bcd(hour));
    TWI_stop();
}

// Read the current time from the RTC
void DS3231_getTime(uint8_t* hour, uint8_t* min, uint8_t* sec) {
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_WRITE);
    TWI_write(0x00);
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_READ);
    *sec = bcd_to_dec(TWI_read_ack());
    *min = bcd_to_dec(TWI_read_ack());
    *hour = bcd_to_dec(TWI_read_nack());
    TWI_stop();
}

// Set alarm 1 on the RTC
void DS3231_setAlarm1(uint8_t hour, uint8_t min, uint8_t sec) {
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_WRITE);
    TWI_write(0x07);
    TWI_write(dec_to_bcd(sec));
    TWI_write(dec_to_bcd(min));
    TWI_write(dec_to_bcd(hour));
    TWI_write(0x80); // Set A1M4 bit to 1 to activate the alarm
    TWI_stop();
}

// Clear alarm 1 flag
void DS3231_clearAlarm1(void) {
    uint8_t status;
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_WRITE);
    TWI_write(0x0F);
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_READ);
    status = TWI_read_nack();
    TWI_stop();
    
    status &= ~0x01;
    
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_WRITE);
    TWI_write(0x0F);
    TWI_write(status);
    TWI_stop();
}

// Enable square wave output
void DS3231_enableSQW(void) {
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_WRITE);
    TWI_write(0x0E); 
    TWI_write(0x40); 
    TWI_stop();
}

// Disable square wave output
void DS3231_disableSQW(void) {
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_WRITE);
    TWI_write(0x0E); 
    TWI_write(0x00); 
    TWI_stop();
}

// Read status register
uint8_t DS3231_readStatus(void) { 
    uint8_t status;
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_WRITE);
    TWI_write(0x0F); 
    TWI_start();
    TWI_write((DS3231_ADDRESS << 1) | TW_READ);
    status = TWI_read_nack();
    TWI_stop();
    return status;
}