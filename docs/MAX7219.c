#include "MAX7219.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>


// Defined font
const uint8_t MAX7219_FONT[] = {
	0,										 // 0
	0,										 // 1
	0,										 // 2
	0,										 // 3
	0,										 // 4
	0,										 // 5
	0,										 // 6
	0,										 // 7
	0,										 // 8
	0,										 // 9
	0,										 // 10
	0,										 // 11
	0,										 // 12
	0,										 // 13
	0,										 // 14
	0,										 // 15
	0,										 // 16
	0,										 // 17
	0,										 // 18
	0,										 // 19
	0,										 // 20
	0,										 // 21
	0,										 // 22
	0,										 // 23
	0,										 // 24
	0,										 // 25
	0,										 // 26
	0,										 // 27
	0,										 // 28
	0,										 // 29
	0,										 // 30
	0,										 // 31
	0,										 // 32
	0,										 // 33
	0,										 // 34
	0,										 // 35
	0,										 // 36
	0,										 // 37
	0,										 // 38
	0,										 // 39
	0,										 // 40
	0,										 // 41
	0,										 // 42
	0,										 // 43
	0,										 // 44
	0,										 // 45
	1, 128,									 // 46
	0,										 // 47
	3, 255, 129, 255,						 // 48
	3, 4, 2, 255,							 // 49
	3, 249, 137, 143,						 // 50
	3, 137, 137, 255,						 // 51
	3, 15, 8, 255,							 // 52
	3, 143, 137, 249,						 // 53
	3, 255, 137, 249,						 // 54
	3, 1, 1, 255,							 // 55
	3, 255, 137, 255,						 // 56
	3, 143, 137, 255,						 // 57
	1, 36,									 // 58
	0,										 // 59
	0,										 // 60
	0,										 // 61
	0,										 // 62
	0,										 // 63
	0,										 // 64
	4, 254, 9, 9, 254,						 // 65
	4, 255, 137, 137, 118,					 // 66
	4, 126, 129, 129, 66,					 // 67
	4, 255, 129, 129, 126,					 // 68
	4, 255, 137, 137, 137,					 // 69
	4, 255, 9, 9, 9,						 // 70
	4, 126, 129, 145, 114,					 // 71
	4, 255, 8, 8, 255,						 // 72
	1, 255,									 // 73
	4, 64, 128, 128, 127,					 // 74
	4, 255, 36, 66, 129,					 // 75
	4, 255, 128, 128, 128,					 // 76
	5, 255, 2, 12, 2, 255,					 // 77
	5, 255, 4, 8, 16, 255,					 // 78
	4, 126, 129, 129, 126,					 // 79
	4, 255, 9, 9, 6,						 // 80
	4, 126, 129, 65, 190,					 // 81
	4, 255, 9, 25, 230,						 // 82
	4, 70, 137, 145, 98,					 // 83
	5, 1, 1, 255, 1, 1,						 // 84
	4, 127, 128, 128, 127,					 // 85
	5, 7, 56, 192, 56, 7,					 // 86
	5, 255, 32, 16, 32, 255,				 // 87
	5, 195, 36, 24, 36, 195,				 // 88
	5, 7, 232, 16, 8, 7,					 // 89
	4, 225, 145, 137, 135,					 // 90
	0,										 // 91
	0,										 // 92
	0,										 // 93
	0,										 // 94
	0,										 // 95
	0,										 // 96
	0,										 // 97
	0,										 // 98
	7, 3, 3, 0, 126, 129, 129, 66,			 // 99
	0,										 // 100
	0,										 // 101
	0,										 // 102
	0,										 // 103
	0,										 // 104
	0,										 // 105
	0,										 // 106
	0,										 // 107
	0,										 // 108
	0,										 // 109
	0,										 // 110
	0,										 // 111
	0,										 // 112
	0,										 // 113
	0,										 // 114
	0,										 // 115
	0,										 // 116
	0,										 // 117
	0,										 // 118
	0,										 // 119
	0,										 // 120
	0,										 // 121
	0,										 // 122
	0,										 // 123
	0,										 // 124
	0,										 // 125
	0,										 // 126
	0,										 // 127
	0,										 // 128
	0,										 // 129
	0,										 // 130
	0,										 // 131
	0,										 // 132
	0,										 // 133
	0,										 // 134
	0,										 // 135
	0,										 // 136
	0,										 // 137
	0,										 // 138
	0,										 // 139
	0,										 // 140
	0,										 // 141
	0,										 // 142
	0,										 // 143
	0,										 // 144
	0,										 // 145
	1, 128,									 // 146
	0,										 // 147
	6, 126, 255, 145, 137, 255, 126,		 // 148
	6, 128, 132, 255, 255, 128, 128,		 // 149
	6, 226, 243, 145, 137, 143, 134,		 // 150
	6, 66, 195, 137, 137, 255, 118,			 // 151
	6, 24, 20, 18, 255, 255, 16,			 // 152
	6, 79, 207, 137, 137, 249, 113,			 // 153
	6, 126, 255, 137, 137, 251, 114,		 // 154
	6, 3, 3, 241, 253, 15, 3,				 // 155
	6, 118, 255, 137, 137, 255, 118,		 // 156
	6, 70, 207, 137, 137, 255, 126,			 // 157
	1, 36,									 // 158
	0,										 // 159
	0,										 // 160
	0,										 // 161
	0,										 // 162
	0,										 // 163
	0,										 // 164
	6, 254, 255, 9, 9, 255, 254,			 // 165
	6, 255, 255, 137, 137, 255, 118,		 // 166
	6, 126, 255, 129, 129, 195, 66,			 // 167
	6, 255, 255, 129, 195, 126, 60,			 // 168
	6, 255, 255, 137, 137, 137, 129,		 // 169
	6, 255, 255, 9, 9, 9, 1,				 // 170
	6, 126, 255, 129, 145, 243, 114,		 // 171
	6, 255, 255, 8, 8, 255, 255,			 // 172
	4, 129, 255, 255, 129,					 // 173
	6, 96, 224, 129, 255, 127, 1,			 // 174
	6, 255, 255, 56, 108, 198, 130,			 // 175
	6, 255, 255, 128, 128, 128, 128,		 // 176
	7, 255, 255, 6, 12, 6, 255, 255,		 // 177
	7, 255, 255, 6, 12, 24, 255, 255,		 // 178
	6, 126, 255, 129, 129, 255, 126,		 // 179
	6, 255, 255, 17, 17, 31, 14,			 // 180
	6, 62, 127, 65, 225, 255, 190,			 // 181
	6, 255, 255, 57, 105, 207, 134,			 // 182
	6, 70, 207, 137, 137, 251, 114,			 // 183
	6, 3, 1, 255, 255, 1, 3,				 // 184
	6, 127, 255, 128, 128, 255, 127,		 // 185
	6, 63, 127, 192, 192, 127, 63,			 // 186
	7, 255, 255, 96, 48, 96, 255, 255,		 // 187
	7, 227, 247, 28, 8, 28, 247, 227,		 // 188
	6, 7, 15, 248, 248, 15, 7,				 // 189
	6, 225, 241, 153, 141, 135, 131,		 // 190
	0,										 // 191
	0,										 // 192
	0,										 // 193
	0,										 // 194
	0,										 // 195
	0,										 // 196
	0,										 // 197
	0,										 // 198
	9, 3, 3, 0, 126, 255, 129, 129, 195, 66, // 199
	0,										 // 200
	0,										 // 201
	0,										 // 202
	0,										 // 203
	0,										 // 204
	0,										 // 205
	0,										 // 206
	0,										 // 207
	0,										 // 208
	0,										 // 209
	0,										 // 210
	0,										 // 211
	0,										 // 212
	0,										 // 213
	0,										 // 214
	0,										 // 215
	0,										 // 216
	0,										 // 217
	0,										 // 218
	0,										 // 219
	0,										 // 220
	0,										 // 221
	0,										 // 222
	0,										 // 223
	0,										 // 224
	0,										 // 225
	0,										 // 226
	0,										 // 227
	0,										 // 228
	0,										 // 229
	0,										 // 230
	0,										 // 231
	0,										 // 232
	0,										 // 233
	0,										 // 234
	0,										 // 235
	0,										 // 236
	0,										 // 237
	0,										 // 238
	0,										 // 239
	0,										 // 240
	0,										 // 241
	0,										 // 242
	0,										 // 243
	0,										 // 244
	0,										 // 245
	0,										 // 246
	0,										 // 247
	0,										 // 248
	0,										 // 249
	0,										 // 250
	0,										 // 251
	0,										 // 252
	0,										 // 253
	0,										 // 254
	0,										 // 255
};

// Array of 32-bit data representing the on and off pixels
uint32_t bitmap[8] = {0};

// Initialize SPI
void SPI_init()
{
	// Set MOSI, SCK, and SS as outputs
	DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
	// Enable SPI, Master, divide clock by 64
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

// Function to transmit data
void SPI_transmit(uint8_t data)
{
	// Start transmission
	SPDR = data;
	// Wait for transmission to complete
	while (!(SPSR & (1 << SPIF)))
		;
}

// Function to send register address and data
void MAX7219_write(uint8_t address, uint8_t data)
{
	PORTB &= ~(1 << PB2);
	SPI_transmit(address);
	SPI_transmit(data);
	PORTB |= (1 << PB2);
}

// Change the brightness of the matrix
void MAX7219_intensity(uint8_t intensityValue)
{
	if (intensityValue > 0x0F)
		intensityValue = 0x0F;

	for (int i = 0; i < devicesNum; i++)
		MAX7219_write(MAX7219_REG_INTENSITY, intensityValue);
}

// Functions to configure the matrix operation
void MAX7219_scanLimit(uint8_t scanLimitValue)
{
	if (scanLimitValue > 0x07)
		scanLimitValue = 0x07;

	for (int i = 0; i < devicesNum; i++)
		MAX7219_write(MAX7219_REG_SCANLIMIT, scanLimitValue);
}

void MAX7219_decodeMode(uint8_t decodeModeValue)
{
	if (decodeModeValue > 0x01)
		decodeModeValue = 0x01;

	for (int i = 0; i < devicesNum; i++)
		MAX7219_write(MAX7219_REG_DECODEMODE, decodeModeValue);
}

void MAX7219_shutdown(uint8_t shutDownValue)
{
	if (shutDownValue > 0x01)
		shutDownValue = 0x01;

	for (int i = 0; i < devicesNum; i++)
		MAX7219_write(MAX7219_REG_SHUTDOWN, shutDownValue);
}

void MAX7219_displayTest(uint8_t displayTestValue)
{
	if (displayTestValue > 0x01)
		displayTestValue = 0x01;

	for (int i = 0; i < devicesNum; i++)
		MAX7219_write(MAX7219_REG_DISPLAYTEST, displayTestValue);
}

// Set all values in the array to 0 to turn off all pixels
void MAX7219_clearBitmap()
{
	for (int i = 0; i < 8; i++)
	{
		bitmap[i] = 0;
	}
}

// Function to display the bitmap on the matrix
void MAX7219_displayBitmap()
{
	for (int i = 1; i < 9; i++)
	{
		PORTB &= ~(1 << PB2);

		for (int j = 0; j <= 3; j++)
		{
			SPI_transmit(i);
			SPCR |= (1 << DORD);
			SPI_transmit((bitmap[8 - i] >> j * 8) & 0xFF);
			SPCR &= ~(1 << DORD);
		}
		PORTB |= (1 << PB2);
	}
}

// Initialize the matrix
void MAX7219_init()
{
	SPI_init();

	MAX7219_scanLimit(0x07);
	MAX7219_decodeMode(0x00);
	MAX7219_intensity(0x01);
	MAX7219_displayTest(0x00);
	MAX7219_clearBitmap();
	MAX7219_shutdown(0x01);
}

// Function to write the lit pixels to the bitmap
// The function takes the ASCII value of the character to find its starting position in the font
// The first number in the font indicates the width of the character, and the rest its value, i.e., which pixels should be lit
void MAX7219_display(char str[32])
{
	uint8_t fontOffset = 100;	// If the offset is 0, another font is used

	MAX7219_clearBitmap();

	int length = strlen(str);
	int pos = 0;

	for (int i = 0; i < length; i++)
	{
		uint16_t startIndex = 0;

		for (int j = 0; j < (str[i] + fontOffset); j++)
		{
			startIndex += MAX7219_FONT[startIndex] + 1;
		}

		uint8_t charLength = MAX7219_FONT[startIndex];

		for (int i = 0; i < charLength; i++)
		{
			uint32_t data = MAX7219_FONT[startIndex + 1 + i];

			for (int j = 0; j < 8; j++)
			{
				bitmap[j] |= (((data >> j) & 0x01) << (31 - pos));
			}
			pos++;
		}
		pos++;
	}
	MAX7219_displayBitmap();
}