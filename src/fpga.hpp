#pragma once

#include <stddef.h>
#include <stdint.h>

// Arduino I/O Pins
#define SCLK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11
#define CS_PIN 10
#define RST_PIN 9

// SPI Speed at 5kHz
#define SPI_SPEED 5000

// FPGA Commands
#define CMD_COUNT ((uint8_t)0xF0)
#define CMD_WRITE ((uint8_t)0xF1)
#define CMD_READ ((uint8_t)0xF2)

// FPGA Response Codes
#define ACK ((uint8_t)0xFA)
#define NACK ((uint8_t)0xFB)
#define FIFO_EMPTY ((uint8_t)0xFE)
#define FIFO_FULL ((uint8_t)0xFF)

namespace fpga
{

void configure_control_pins();

void reset();

void write(uint8_t data[], size_t data_len);

void read(size_t len);

void count();

/*
 * Custom printf-like function for printing formatted output
 * to the Serial console.
 *
 * Supported format specifiers:
 * - %c: writes a single character
 * - %d: writes a signed integer
 * - %i: Same as %d
 * - %u: writes an unsigned integer
 * - %x: writes a hexadecimal number, prefixed with "0x"
 * - %s: writes a null-terminated string
 * - %k: writes FPGA command constants (CMD_COUNT, CMD_WRITE, CMD_READ)
 * - Any byte >= 0xFA is interpreted as FPGA response code (ACK, FIFO_FULL, ...)
 */
void printf(const char *fmt, ...);

} // namespace fpga
