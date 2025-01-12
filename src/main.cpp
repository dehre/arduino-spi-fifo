#include "ExtSerial.hpp"
#include "fpga.hpp"
#include <Arduino.h>

void setup()
{
    ExtSerial.begin(9600);

    fpga::setup_io();

    // clang-format off
    fpga::setup_callbacks({
        .on_command = [](uint8_t sent, uint8_t recv) {
            ExtSerial.printf("%k [%x]\n", sent, recv);
        },
        .on_count = [](uint8_t sent, uint8_t recv) {
            ExtSerial.printf("  CNT: %d\n", recv);
        },
        .on_write = [](uint8_t sent, uint8_t recv) {
            ExtSerial.printf("   WR: %d [%x]\n", sent, recv);
        },
        .on_read = [](uint8_t sent, uint8_t recv) {
            ExtSerial.printf("   RD: %d\n", recv);
        },
        .after_each = []() {
            delay(1000); /* 1s */
        }
    });
    // clang-format on

    /* Wait until both the Arduino and the FPGA are plugged in... */
    delay(5000); /* 5s */

    fpga::reset();
}

void loop()
{
    ExtSerial.printf("\nPush 50 items, Count, Pop 10 items...\n");
    uint8_t data_a[50];
    for (size_t i = 0; i < sizeof(data_a); ++i)
        data_a[i] = 1 + i;
    fpga::write(data_a, sizeof(data_a));
    fpga::count();
    fpga::read(10);

    ExtSerial.printf("\nPush 61 items, Count, Pop 100 items...\n");
    uint8_t data_b[61];
    for (size_t i = 0; i < sizeof(data_b); ++i)
        data_b[i] = 51 + i;
    fpga::write(data_b, sizeof(data_b));
    fpga::count();
    fpga::read(100);

    delay(10000); /* 10s */
}
