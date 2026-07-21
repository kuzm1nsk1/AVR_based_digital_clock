# ---- Configuration ----
MCU        = atmega328p
F_CPU      = 16000000UL
PROGRAMMER = usbasp
TARGET     = avr_clock
BUILD_DIR  = build

# ---- Tools ----
CC      = avr-gcc
OBJCOPY = avr-objcopy
SIZE    = avr-size
AVRDUDE = avrdude

# ---- Flags ----
# F_CPU is defined here (not only in a header) so it is set before any
# <util/delay.h> include, regardless of include order in the sources.
CFLAGS  = -std=gnu11 -Os -Wall -Wextra -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Iinclude
# -lm: the temperature averaging in main.c uses round() from <math.h>
LDFLAGS = -mmcu=$(MCU) -lm

# ---- Sources ----
SRC  = $(wildcard src/*.c)
OBJ  = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRC))
ELF  = $(BUILD_DIR)/$(TARGET).elf
HEX  = $(TARGET).hex

# ---- Rules ----
all: $(HEX)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

size: $(ELF)
	$(SIZE) --mcu=$(MCU) -C $(ELF)

flash: $(HEX)
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(HEX):i

# ATmega328P @ 16 MHz external crystal, brown-out on. Adjust to your board.
fuses:
	@echo "Example fuses for a 16 MHz crystal:"
	@echo "$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m"

clean:
	rm -rf $(BUILD_DIR) $(HEX)

.PHONY: all size flash fuses clean
