# Firmware

Bare-metal C for a standalone **ATmega328P** @ 16 MHz. No Arduino core — direct
register access, with small driver modules for each peripheral.

```
firmware/
├── src/
│   ├── main.c            menu logic, timers, ISRs
│   ├── MAX7219.c         8x32 LED matrix driver (SPI, bit-banged font)
│   ├── ds3231.c          RTC over I2C (time, alarm, 1 Hz SQW)
│   ├── rotary_encoder.c  quadrature decode + button
│   └── tmp36.c           ADC read + temperature conversion
├── include/              matching headers
└── Makefile
```

## Toolchain

| Tool     | Notes                        |
|----------|------------------------------|
| avr-gcc  | Zak Kemble build (Windows) or distro package |
| avr-libc | bundled with the toolchain   |
| avrdude  | for flashing                 |
| make     | build driver                 |

The only standard-library headers used are `stdio.h`, `stdint.h`, `math.h`,
and the AVR headers (`avr/io.h`, `avr/interrupt.h`, `util/delay.h`,
`avr/eeprom.h`). `math.h` (for `round()`) is linked with `-lm`, already set in
the Makefile.

## Configure

Edit the top of the `Makefile` if needed:

```make
MCU        = atmega328p
F_CPU      = 16000000UL
PROGRAMMER = usbasp
```

`F_CPU` is defined via the Makefile (`-DF_CPU`) so it is set before any
`<util/delay.h>` include, regardless of include order.

## Build

```sh
cd firmware
make            # -> avr_clock.hex (verified: ~7.4 kB flash, 22.7% of the 328P)
make size       # flash/RAM usage
make clean
```

## Flash

```sh
make flash      # avrdude write via the programmer set above
make fuses      # prints an example fuse command for a 16 MHz crystal
```

> The `make fuses` target only prints a suggested command — review the fuse
> values against your board before writing them.
