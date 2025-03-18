#include "rotary_encoder.h"
#include <util/delay.h>

#define ENC_A_PIN PD2      
#define ENC_B_PIN PD3
#define ENC_BUTTON_PIN PD4 

volatile int16_t encoder_position = 0;
volatile int16_t enc_counter = 0;
volatile uint8_t last_state = 0;
volatile int16_t prev_position = 0;

// State machine table
const int8_t ENCODER_STATES[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

// Encoder initialization
void encoder_init(void)
{
    // Set ENC_A_PIN and ENC_B_PIN as inputs
    DDRD &= ~((1 << ENC_A_PIN) | (1 << ENC_B_PIN));
    // Enable pull-up resistors
    PORTD |= (1 << ENC_A_PIN) | (1 << ENC_B_PIN);
    // Set ENC_BUTTON_PIN as input
    DDRD &= ~(1 << ENC_BUTTON_PIN);
    // Enable pull-up resistor
    PORTD |= (1 << ENC_BUTTON_PIN);

    // Enable external interrupts on INT0 and INT1 (ENC_A_PIN and ENC_B_PIN)
    EICRA |= (1 << ISC00) | (1 << ISC10); // Trigger on any logical change
    EIMSK |= (1 << INT0) | (1 << INT1);

    // Read the initial state of the encoder
    last_state = (PIND & ((1 << ENC_A_PIN) | (1 << ENC_B_PIN))) >> ENC_A_PIN;
}

// Update encoder position
void update_encoder_position(void)
{
    // Read the current state
    uint8_t current_state = (PIND & ((1 << ENC_A_PIN) | (1 << ENC_B_PIN))) >> ENC_A_PIN;
    // Determine state transition
    uint8_t state_transition = (last_state << 2) | current_state;
    // Update encoder position
    int8_t movement = ENCODER_STATES[state_transition & 0x0F];
    encoder_position += movement;

    last_state = current_state;
    // Update enc_counter only if 4 states have passed
    if (encoder_position > 3)
    {
        enc_counter++;
        encoder_position = 0;
    }
    else if (encoder_position < -3)
    {
        enc_counter--;
        encoder_position = 0;
    }
}

// ISR for ENC_A_PIN
ISR(INT0_vect)
{
    update_encoder_position();
}

// ISR for ENC_B_PIN
ISR(INT1_vect)
{
    update_encoder_position();
}

// Read the direction of encoder rotation
int16_t encoder_read(void)
{
    int16_t position;
    int8_t direction = 0; 
    cli();
    position = enc_counter;

    if (position != prev_position)
    {
        direction = position > prev_position ? 1 : -1;
        prev_position = position;
    }

    sei();
    return direction;
}

// Read the state of the button
uint8_t encoder_button_pressed(void)
{
    if (!(PIND & (1 << ENC_BUTTON_PIN)))
    {
        _delay_ms(10);
        if (!(PIND & (1 << ENC_BUTTON_PIN)))
        {
            while (!(PIND & (1 << ENC_BUTTON_PIN)))
                ;
            return 1;
        }
    }
    return 0;
}