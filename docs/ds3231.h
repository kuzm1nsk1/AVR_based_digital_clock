#ifndef DS3231_H
#define DS3231_H

#include <avr/io.h>

// DS3231 I2C address
#define DS3231_ADDRESS 0x68

void DS3231_init(void);
void DS3231_setTime(uint8_t hour, uint8_t min, uint8_t sec);
void DS3231_getTime(uint8_t* hour, uint8_t* min, uint8_t* sec);
void DS3231_setAlarm1(uint8_t hour, uint8_t min, uint8_t sec);
void DS3231_clearAlarm1(void);
void DS3231_enableSQW(void);
void DS3231_disableSQW(void);
uint8_t DS3231_readStatus(void);

#endif