#pragma once

#include <stddef.h>
#include <stdint.h>

/* Arduino Uno I/O Pins */
#define SCK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11
#define CS_PIN 10
#define RST_PIN 9

/* SPI Speed at 5kHz */
#define SPI_SPEED 5000

/* FPGA Commands */
#define CMD_COUNT ((uint8_t)0xF0)
#define CMD_WRITE ((uint8_t)0xF1)
#define CMD_READ ((uint8_t)0xF2)

/* FPGA Response Codes */
#define ACK ((uint8_t)0xFA)
#define NACK ((uint8_t)0xFB)
#define FIFO_EMPTY ((uint8_t)0xFE)
#define FIFO_FULL ((uint8_t)0xFF)

namespace fpga
{

struct callbacks
{
    void (*on_command)(uint8_t sent, uint8_t recv);
    void (*on_count)(uint8_t sent, uint8_t recv);
    void (*on_write)(uint8_t sent, uint8_t recv);
    void (*on_read)(uint8_t sent, uint8_t recv);
    void (*after_each)();
};

/*
 * Setup SPI and I/O pins for communication.
 */
void setup_io();

/*
 * Setup custom callbacks on count, write, and read
 * to allow for customized behavior without updating
 * the fpga module.
 */
void setup_callbacks(struct callbacks cb);

/*
 * Reset the FPGA.
 */
void reset();

/*
 * Get the number of elements stored in the FIFO.
 */
void count();

/*
 * Write data_len items to the FIFO.
 */
void write(uint8_t data[], size_t data_len);

/*
 * Read len items from the FIFO.
 */
void read(size_t len);

} // namespace fpga
