#include "fpga.hpp"
#include <Arduino.h>

void setup()
{
    Serial.begin(9600);

    fpga::configure_control_pins();

    // Wait until both Arduino and FPGA are plugged in, then reset the FPGA
    delay(5000);
    fpga::reset();
}

void loop()
{
    Serial.println("\nWRITING ALL, READING ALL!\n");
    uint8_t data_full_1[] = {10, 20, 30, 40, 50, 60, 70};
    fpga::write(data_full_1, sizeof(data_full_1));
    delay(1000);
    fpga::read(sizeof(data_full_1));
    delay(1000);

    Serial.println("\nWRITING  4, READING 3, WRITING 2, READING ALL!\n");
    uint8_t data_partial_1[] = {3, 6, 9, 12};
    fpga::write(data_partial_1, sizeof(data_partial_1));
    delay(1000);
    fpga::read(3);
    delay(1000);
    uint8_t data_partial_2[] = {73, 76};
    fpga::write(data_partial_2, sizeof(data_partial_2));
    delay(1000);
    fpga::read(7);
    delay(1000);

    Serial.println("\nWRITING ALL, READING 1, READING ALL!\n");
    uint8_t data_full_2[] = {7, 14, 21, 28, 35, 42};
    fpga::write(data_full_2, sizeof(data_full_2));
    delay(1000);
    fpga::read(1);
    delay(1000);
    fpga::read(5);
    delay(1000);

    delay(10000); // 10s
}
