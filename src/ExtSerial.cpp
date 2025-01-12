#include "ExtSerial.hpp"
#include "fpga.hpp"

_ExtSerial::_ExtSerial(HardwareSerial &hw_serial) : m_hw_serial{hw_serial} {};

void _ExtSerial::begin(unsigned long baud)
{
    m_hw_serial.begin(baud);
}

void _ExtSerial::begin(unsigned long baud, uint8_t config)
{
    m_hw_serial.begin(baud, config);
}

size_t _ExtSerial::printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, NULL);

    size_t n = 0;
    const char *p = fmt;
    do
    {
        if (*p != '%')
        {
            n += m_hw_serial.print(*p);
            continue;
        }
        p++;

        int arg = va_arg(args, int);
        if ((uint8_t)arg >= 0xFA)
        {
            n += print_res_code((uint8_t)arg);
            continue;
        }

        switch (*p)
        {
        case 'c':
            n += m_hw_serial.print((char)arg);
            break;
        case 'd':
        case 'i':
            n += m_hw_serial.print(arg, DEC);
            break;
        case 'k':
            n += print_cmd((uint8_t)arg);
            break;
        case 's':
            n += m_hw_serial.print((char *)arg);
            break;
        case 'x':
            n += m_hw_serial.print("0x");
            n += m_hw_serial.print(arg, HEX);
            break;
        case 'u':
            n += m_hw_serial.print((unsigned)arg);
            break;
        default:
            n += m_hw_serial.print('?');
        }
    } while (*(++p) != '\0');

    va_end(args);
    return n;
}

//
// Private Methods
//

size_t _ExtSerial::print_cmd(uint8_t byte)
{
    switch (byte)
    {
    case CMD_COUNT:
        return m_hw_serial.print("CMD_COUNT");
    case CMD_WRITE:
        return m_hw_serial.print("CMD_WRITE");
    case CMD_READ:
        return m_hw_serial.print("CMD_READ");
    default:
        return m_hw_serial.print("?");
    }
}

size_t _ExtSerial::print_res_code(uint8_t byte)
{
    switch (byte)
    {
    case ACK:
        return m_hw_serial.print("ACK");
    case NACK:
        return m_hw_serial.print("NACK");
    case FIFO_EMPTY:
        return m_hw_serial.print("FIFO_EMPTY");
    case FIFO_FULL:
        return m_hw_serial.print("FIFO_FULL");
    default:
        return m_hw_serial.print("?");
    }
}

_ExtSerial ExtSerial(Serial);
