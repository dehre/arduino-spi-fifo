#include "fpga.hpp"
#include <Arduino.h>
#include <SPI.h>
#include <stdarg.h>

static size_t print_cmd(uint8_t byte);
static size_t print_res_code(uint8_t byte);

//
// Public Functions
//

namespace fpga
{
void configure_control_pins()
{
    pinMode(RST_PIN, OUTPUT);
    digitalWrite(RST_PIN, LOW);

    digitalWrite(CS_PIN, HIGH);
    pinMode(CS_PIN, OUTPUT);

    SPI.begin();
}

void reset()
{
    digitalWrite(RST_PIN, HIGH);
    delay(1);
    digitalWrite(RST_PIN, LOW);
}

void write(uint8_t data[], size_t data_len)
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);

    SPI.transfer(CMD_WRITE);
    uint8_t res = SPI.transfer(0);
    fpga::printf("out: %k \t-> in: %x\n", CMD_WRITE, res);

    for (size_t i = 0; i < data_len; i++)
    {
        res = SPI.transfer(data[i]);
        fpga::printf("out: %d \t\t-> in: %d\n", data[i], res);
    }

    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}

void read(size_t len)
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);

    SPI.transfer(CMD_READ);
    uint8_t res = SPI.transfer(0);
    fpga::printf("out: %k \t-> in: %x\n", CMD_READ, res);

    for (size_t i = 0; i < len; i++)
    {
        res = SPI.transfer(0);
        fpga::printf("out: %d \t\t-> in: %d\n", 0, res);
    }

    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}

void count()
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);

    SPI.transfer(CMD_COUNT);
    uint8_t res = SPI.transfer(0);
    fpga::printf("out: %k \t-> in: %x\n", CMD_COUNT, res);

    res = SPI.transfer(0);
    fpga::printf("out: %d \t\t-> in: %d\n", 0, res);

    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}

void printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, NULL);

    const char *p = fmt;
    do
    {
        if (*p != '%')
        {
            Serial.print(*p);
            continue;
        }
        p++;

        int arg = va_arg(args, int);
        if ((uint8_t)arg >= 0xFA)
        {
            print_res_code((uint8_t)arg);
            continue;
        }

        switch (*p)
        {
        case 'c':
            Serial.print((char)arg);
            break;
        case 'd':
        case 'i':
            Serial.print(arg, DEC);
            break;
        case 'k':
            print_cmd((uint8_t)arg);
            break;
        case 's':
            Serial.print((char *)arg);
            break;
        case 'x':
            Serial.print("0x");
            Serial.print(arg, HEX);
            break;
        case 'u':
            Serial.print((unsigned)arg);
            break;
        default:
            Serial.print('?');
        }
    } while (*(++p) != '\0');
    va_end(args);
}
} // namespace fpga

//
// Private Functions
//

static size_t print_cmd(uint8_t byte)
{
    switch (byte)
    {
    case CMD_COUNT:
        return Serial.print("CMD_STATUS");
    case CMD_WRITE:
        return Serial.print("CMD_WRITE");
    case CMD_READ:
        return Serial.print("CMD_READ");
    default:
        return Serial.print("?");
    }
}

static size_t print_res_code(uint8_t byte)
{
    switch (byte)
    {
    case ACK:
        return Serial.print("ACK");
    case NACK:
        return Serial.print("NACK");
    case FIFO_EMPTY:
        return Serial.print("FIFO_EMPTY");
    case FIFO_FULL:
        return Serial.print("FIFO_FULL");
    default:
        return Serial.print("?");
    }
}
