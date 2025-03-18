#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include "rotary_encoder.h"
#include "MAX7219.h"
#include "ds3231.h"
#include "tmp36.h"


int8_t menuActive = 0;
int8_t menuPos = 0;
int8_t subMenuPos = 0;
int8_t hour, min, sec;
int8_t brightness = 0;
uint8_t alarmActive = 0;
uint8_t allSegmetsOn = 0;
volatile uint8_t prevPinState = 0;
volatile uint32_t milliseconds = 0;
uint32_t prevTime = 0;
volatile float averageTemperature = 0.0;
volatile uint8_t sampleNum = 0;
uint8_t displayTime = 1;

// Function prototypes
void timer0_init(void);
void buzz(void);
void buzz_stop(void);
void display(void);
void clockwise(void);
void counterClockwise(void);
void buttonPress(void);



int main(void)
{
    
    timer0_init();
    adc_init();
    encoder_init();
    MAX7219_init();
    DS3231_init();
    DS3231_enableSQW();

    brightness = eeprom_read_byte((uint8_t *)0x01);
    MAX7219_intensity((uint8_t)brightness);

    DDRB &= ~(1 << PB0);
    PORTB |= (1 << PORTB0);
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0);
    prevPinState = PINB & (1 << PB0);

    sei();

    while (1)
    {
        // Function for digit blinking when setting time or alarm
        if (menuActive == 1 || menuActive == 2)
        {
            if (milliseconds - prevTime > 500)
            {
                displayTime = !displayTime;
                prevTime = milliseconds;
                display();
            }
        }

        // Function for display blinking when alarm goes off
        if (alarmActive)
        {
            if (milliseconds - prevTime > 500)
            {
                prevTime = milliseconds;
                allSegmetsOn = !allSegmetsOn;
                if (allSegmetsOn)
                {
                    MAX7219_displayTest(1);
                    buzz();
                }
                else
                {
                    MAX7219_display("");
                    MAX7219_displayTest(0);
                    buzz_stop();
                }
            }
        }
        // Call function based on encoder button press
        if (encoder_button_pressed())
            buttonPress();

        int8_t direction = encoder_read();

        if (direction == 1)
            clockwise();
        else if (direction == -1)
            counterClockwise();
    }
}

// Function to track the number of milliseconds since the program started
ISR(TIMER0_OVF_vect)
{
    TCNT0 = 6;
    milliseconds++;
}

// Timer0 initialization
void timer0_init()
{
    // Set Timer0 to normal mode
    TCCR0A &= ~((1 << WGM01) | (1 << WGM00));
    // Prescaler set to 64
    TCCR0B = (1 << CS01) | (1 << CS00);
    // Enable Timer0 overflow interrupt
    TIMSK0 |= (1 << TOIE0);
    // Calculated value for 1ms
    TCNT0 = 6;
}

// Function to generate a 60Hz square wave signal for the speaker
void buzz()
{
    // Set PB1 as output
    DDRB |= (1 << PB1);
    // Toggle OC1A on compare match
    TCCR1A = (1 << COM1A0);
    // Set Timer 1 to CTC mode and prescaler to 1024
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    // Calculated value for 60Hz
    OCR1A = 129;
}

// Function to stop the 60Hz signal
void buzz_stop()
{
    // Disable Timer1
    TCCR1B &= ~((1 << CS12) | (1 << CS10));
    // Set PB1 to GND
    PORTB &= ~(1 << PB1);
}

// Function to display a given string on the LED matrix
void display()
{
    char str[32];
    // Depending on the active menu, the given string is displayed
    switch (menuActive)
    {
    // Menu for displaying time and temperature and other sub-menus
    case 0:
        switch (menuPos)
        {
        case 0:
            cli(); // Disable interrupts to avoid affecting time retrieval from RTC
            DS3231_getTime((uint8_t *)&hour, (uint8_t *)&min, (uint8_t *)&sec);
            // After 40 samples, the average temperature is calculated and displayed for 1 second
            if (sampleNum >= 40)
            {
                averageTemperature /= (float)sampleNum;
                sprintf(str, "   %d c", (int)(round(averageTemperature)));
                averageTemperature = 0;
                sampleNum = 0;
            }
            else
            {
                if (sec % 2 == 0)   // On even seconds, a colon is displayed, on odd seconds it is not
                    hour < 10 ? sprintf(str, "%5d : %02d", hour, min) : sprintf(str, "%2d : %02d", hour, min);
                else
                    hour < 10 ? sprintf(str, "%5d    %02d", hour, min) : sprintf(str, "%2d    %02d", hour, min);
            }
            sei();
            break;
        // Display if we want to select time setting
        case 1:
            sprintf(str, "CLK");
            break;
        // Display if we want to select alarm setting
        case 2:
            sprintf(str, "ALRM");
            break;
        // Display if we want to select brightness setting
        case 3:
            sprintf(str, "BRT");
            break;
        }
        break;
    // Menus for setting time and alarm
    case 1:
    case 2:
        // If hours or minutes need to be displayed, the if part is executed, otherwise the else part
        if (displayTime)
        {
            if (hour < 10)
                sprintf(str, "%5d : %02d", hour, min);
            else
                sprintf(str, "%2d : %02d", hour, min);
        }
        else
        {
            if (hour < 10)
                subMenuPos ? sprintf(str, "            : %02d", min) : sprintf(str, "%5d :   ", hour);
            else
                subMenuPos ? sprintf(str, "               : %02d", min) : sprintf(str, "%2d :", hour);
        }
        break;
    // Menu for setting screen brightness
    case 3:
        sprintf(str, "12 : 34");
        break;
    }
    // The string is displayed only if the alarm is not active
    if (!alarmActive)
        MAX7219_display(str);
}

// Function called when the encoder is turned to the right
void clockwise()
{
    switch (menuActive)
    {
    // Turning in the first menu displays other sub-menus
    case 0:
        menuPos++;
        if (menuPos > 3)
            menuPos = 0;
        break;
    // Turning in the time or alarm setting menu increases the number of minutes or hours
    case 1:
    case 2:
        // prevTime takes the current time value so that the digits do not blink when changing hours or minutes
        prevTime = milliseconds;
        // displayTime is set to 1 so that hours or minutes are immediately displayed when turning
        displayTime = 1;
        // Depending on the position, hours or minutes are increased, entering the menu at position 0
        subMenuPos == 0 ? min++ : hour++;
        if (min > 59)
        {
            min = 0;
            hour++;
        }
        if (hour > 23)
        {
            hour = 0;
        }
        break;
    // Turning in the brightness menu increases the brightness, and the data is sent to the LED matrix
    case 3:
        brightness++;
        if (brightness > 15)
            brightness = 15;
        MAX7219_intensity((uint8_t)brightness);
        break;
    }
    // After turning, the changes are immediately displayed on the screen
    display();
}

// Function called when the encoder is turned to the left
void counterClockwise()
{
    switch (menuActive)
    {
    case 0:
        menuPos--;
        if (menuPos < 0)
            menuPos = 3;
        break;

    case 1:
    case 2:
        prevTime = milliseconds;
        displayTime = 1;
        subMenuPos == 0 ? min-- : hour--;
        if (min < 0)
        {
            min = 59;
            hour--;
        }
        if (hour < 0)
        {
            hour = 23;
            min = 59;
        }
        break;

    case 3:
        brightness--;
        if (brightness < 0)
            brightness = 0;
        MAX7219_intensity((uint8_t)brightness);
        break;
    }

    display();
}

// Function called when the encoder button is pressed
void buttonPress()
{
    cli();  // Disable interrupts to avoid interfering with other functions
    // If the alarm is active, deactivate it
    if (alarmActive)
    {
        MAX7219_displayTest(0);
        buzz_stop();
        alarmActive = 0;
        menuActive = 0;
        menuPos = 0;
    }

    switch (menuActive)
    {
    
    case 0:
        // In the first menu, confirm the selected sub-menu
        menuActive = menuPos;
        break;
    
    case 1:
        // In the time setting menu, move the selection from minutes to hours
        subMenuPos++;
        // If hours are confirmed, set the time and return to the first menu
        if (subMenuPos == 2)
        {
            DS3231_setTime((uint8_t)hour, (uint8_t)min, 0);
            subMenuPos = 0;
            menuActive = 0;
            menuPos = 0;
        }
        break;

    case 2:
        // In the alarm setting menu, move the selection from minutes to hours
        subMenuPos++;
        // If hours are confirmed, set the alarm and return to the first menu
        if (subMenuPos == 2)
        {
            DS3231_setAlarm1((uint8_t)hour, (uint8_t)min, 0);
            subMenuPos = 0;
            menuActive = 0;
            menuPos = 0;
        }
        break;

    case 3:
        // The brightness value is written to the EEPROM and returns to the first menu
        eeprom_write_byte((uint8_t *)0x01, (uint8_t)brightness);
        subMenuPos = 0;
        menuActive = 0;
        menuPos = 0;
        break;
    }
    display();
    sei();
}

// Interrupt routine caused by RTC
ISR(PCINT0_vect)
{   
    // A temperature sample is taken and added to the previous ones
    averageTemperature += read_temperature();
    sampleNum++;

    // Check if PB0 is logical 0
    if (!(PINB & (1 << PINB0)))
    {
        // If the previous state was logical 1, then this is a falling edge
        if (prevPinState & (1 << PINB0))
        {
            // Check if the alarm flag is active in the RTC
            uint8_t status = DS3231_readStatus();
            if (status & 0x01)
            {
                // If the alarm flag is active, clear it and activate the alarm
                alarmActive = 1;
                menuActive = -1;
                DS3231_clearAlarm1();
            }
            display();
        }
    }
    // Save the signal state
    prevPinState = PINB & (1 << PB0);
}
