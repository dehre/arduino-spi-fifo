#include "fpga.hpp"
#include <Arduino.h>
#include <SPI.h>
#include <stdarg.h>

static fpga::callbacks _callbacks = {
    .on_command = [](uint8_t sent, uint8_t recv) {},
    .on_count = [](uint8_t sent, uint8_t recv) {},
    .on_write = [](uint8_t sent, uint8_t recv) {},
    .on_read = [](uint8_t sent, uint8_t recv) {},
    .after_each = []() {}};

namespace fpga
{
void setup_io()
{
    pinMode(RST_PIN, OUTPUT);
    digitalWrite(RST_PIN, LOW);

    digitalWrite(CS_PIN, HIGH);
    pinMode(CS_PIN, OUTPUT);

    SPI.begin();
}

void setup_callbacks(struct callbacks cb)
{
    _callbacks = cb;
}

void reset()
{
    digitalWrite(RST_PIN, HIGH);
    delay(1);
    digitalWrite(RST_PIN, LOW);
}

void count()
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);

    SPI.transfer(CMD_COUNT);
    uint8_t res = SPI.transfer(0);
    _callbacks.on_command(CMD_COUNT, res);

    res = SPI.transfer(0);
    _callbacks.on_count(0, res);

    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
    _callbacks.after_each();
}

void write(uint8_t data[], size_t data_len)
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);

    SPI.transfer(CMD_WRITE);
    uint8_t res = SPI.transfer(0);
    _callbacks.on_command(CMD_WRITE, res);

    for (size_t i = 0; i < data_len; i++)
    {
        res = SPI.transfer(data[i]);
        _callbacks.on_write(data[i], res);
    }

    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
    _callbacks.after_each();
}

void read(size_t len)
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);

    SPI.transfer(CMD_READ);
    uint8_t res = SPI.transfer(0);
    _callbacks.on_command(CMD_READ, res);

    for (size_t i = 0; i < len; i++)
    {
        res = SPI.transfer(0);
        _callbacks.on_read(0, res);
    }

    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
    _callbacks.after_each();
}

} // namespace fpga
