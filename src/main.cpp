#include <SPI.h>

#define SPI_SPEED 5000 // 5kHz

// Arduino I/O Pins
#define SCLK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11
#define CS_PIN 10
#define RST_PIN 9

// FPGA Commands
#define CMD_COUNT 0xF0
#define CMD_WRITE 0xF1
#define CMD_READ 0xF2

// FPGA Responses
#define ACK 0xFA
#define NACK 0xFB
#define FIFO_EMPTY 0xFE
#define FIFO_FULL 0xFF

void begin_spi()
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_PIN, LOW);
}

void end_spi()
{
    digitalWrite(CS_PIN, HIGH);
    SPI.endTransaction();
}

void write_spi(uint8_t data[], size_t data_len)
{
    begin_spi();
    SPI.transfer(CMD_WRITE);
    delay(10);
    uint8_t writing_response = SPI.transfer(0);
    Serial.print("Writing response: 0x");
    Serial.println(writing_response, HEX);
    delay(10);
    for (size_t i = 0; i < data_len; i++)
    {
        uint8_t bit_read = SPI.transfer(data[i]);
        Serial.print("Writing: ");
        Serial.print(data[i], DEC);
        Serial.print("; Response: 0x");
        Serial.println(bit_read, HEX);
        delay(10); // 10ms
    }
    end_spi();
}

void read_spi(size_t len)
{
    begin_spi();
    SPI.transfer(CMD_READ);
    delay(10);
    uint8_t reading_response = SPI.transfer(0);
    Serial.print("Reading response: 0x");
    Serial.println(reading_response, HEX);
    delay(10);
    for (size_t i = 0; i < len; i++)
    {
        uint8_t bit_read = SPI.transfer(0);
        Serial.print("Read: ");
        Serial.println(bit_read, DEC);
        delay(10); // 10ms
    }
    end_spi();
}

void count_spi()
{
    begin_spi();
    SPI.transfer(CMD_COUNT);
    delay(10);
    uint8_t count_response = SPI.transfer(0);
    Serial.print("Count response: 0x");
    Serial.println(count_response, HEX);
    delay(10);

    uint8_t bit_read = SPI.transfer(0);
    Serial.print("Count: ");
    Serial.println(bit_read, DEC);
    delay(10);
    end_spi();
}

void reset_spi()
{
    digitalWrite(RST_PIN, HIGH);
    delay(1);
    digitalWrite(RST_PIN, LOW);
}

void setup()
{
    // Initialize Arduino pins
    pinMode(RST_PIN, OUTPUT);
    pinMode(SCLK_PIN, OUTPUT);
    pinMode(CS_PIN, OUTPUT);

    digitalWrite(RST_PIN, LOW);
    digitalWrite(SCLK_PIN, LOW);
    digitalWrite(CS_PIN, HIGH);

    // Wait until both Arduino and FPGA are plugged in, then reset the FPGA
    delay(5000);
    reset_spi();

    // Initialize Arduino peripherals
    Serial.begin(9600);
    SPI.begin();
}

void loop()
{
    Serial.println("WRITING ALL, READING ALL!");
    uint8_t data_full_1[100];
    for (size_t i = 0; i < sizeof(data_full_1); i++)
        data_full_1[i] = (i + 1);
    write_spi(data_full_1, sizeof(data_full_1));
    delay(1000);
    read_spi(sizeof(data_full_1));
    delay(1000);

    Serial.println("WRITING  4, READING 3, WRITING 2, READING ALL!");
    uint8_t data_partial_1[] = {3, 6, 9, 12};
    write_spi(data_partial_1, sizeof(data_partial_1));
    delay(1000);
    read_spi(3);
    delay(1000);
    uint8_t data_partial_2[] = {73, 76};
    write_spi(data_partial_2, sizeof(data_partial_2));
    delay(1000);
    read_spi(7);
    delay(1000);

    Serial.println("WRITING ALL, READING 1, READING ALL!");
    for (size_t i = 0; i < sizeof(data_full_1); i++)
        data_full_1[i] = data_full_1[i] * 2;
    write_spi(data_full_1, sizeof(data_full_1));
    delay(1000);
    read_spi(1);
    delay(1000);
    read_spi(sizeof(data_full_1));
    delay(1000);

    delay(10000); // 10s
}
