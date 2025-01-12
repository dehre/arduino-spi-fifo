#pragma once

#include <HardwareSerial.h>

class _ExtSerial
{
  public:
    _ExtSerial(HardwareSerial &hw_serial);

    void begin(unsigned long baud);
    void begin(unsigned long baud, uint8_t config);

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
     * - Any byte >= 0xFA is interpreted as FPGA resp-code (ACK, FIFO_FULL, ...)
     */
    size_t printf(const char *fmt, ...);

  private:
    HardwareSerial &m_hw_serial;
    size_t print_cmd(uint8_t byte);
    size_t print_res_code(uint8_t byte);
};

extern _ExtSerial ExtSerial;
