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
        }
    });
    // clang-format on

    // Wait until both the Arduino and the FPGA are plugged in...
    delay(5000);

    fpga::reset();
}

void loop()
{
    ExtSerial.printf("\nWriting 7 items, getting count, reading 7 items...\n");
    uint8_t data_a[] = {10, 20, 30, 40, 50, 60, 70};
    fpga::write(data_a, sizeof(data_a));
    delay(1000);
    fpga::count();
    delay(1000);
    fpga::read(sizeof(data_a));
    delay(1000);

    ExtSerial.printf("\nWriting 4 items, reading 3 items, writing 2 items, "
                     "getting count, reading 3 items...\n");
    uint8_t data_b[] = {3, 6, 9, 12};
    fpga::write(data_b, sizeof(data_b));
    delay(1000);
    fpga::read(3);
    delay(1000);
    uint8_t data_c[] = {73, 76};
    fpga::write(data_c, sizeof(data_c));
    delay(1000);
    fpga::count();
    delay(1000);
    fpga::read(3);
    delay(1000);

    ExtSerial.printf("\nWriting 6 items, reading 1 item, reading 6 items\n");
    uint8_t data_d[] = {7, 14, 21, 28, 35, 42};
    fpga::write(data_d, sizeof(data_d));
    delay(1000);
    fpga::read(1);
    delay(1000);
    fpga::read(6);
    delay(1000);

    delay(10000); // 10s
}
