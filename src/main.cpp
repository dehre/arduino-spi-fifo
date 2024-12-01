#include <SPI.h>

#define SPI_SPEED 500000 // 500kHz
#define SPI_CLK_PIN 13
#define SPI_MISO_PIN 12
#define SPI_MOSI_PIN 11
#define SPI_CS_PIN 10
#define FPGA_RESET_PIN 9

#define CMD_STATUS 0xFA
#define CMD_READ   0xFB
#define CMD_WRITE  0xFC

#define ACK        0xAA
#define NACK       0xBB
#define EMPTY_BYTE 0xFE
#define FULL_BYTE  0xFF

void begin_spi()
{
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));
    digitalWrite(SPI_CS_PIN, LOW);
}

void end_spi()
{
    digitalWrite(SPI_CS_PIN, HIGH);
    SPI.endTransaction();
}

void test_status()
{
    begin_spi();
    SPI.transfer(CMD_STATUS);
    delay(10);
    for (int i = 10; i <= 50; i += 10)
    {
        uint8_t bit_read = SPI.transfer(0);
        Serial.print("Bit read: ");
        Serial.println(bit_read, DEC);
        delay(10); // 10ms
    }
    end_spi();

    delay(5000); // 5s
}

void test_set_get()
{
    {
        begin_spi();
        SPI.transfer(CMD_WRITE);
        delay(10);
        uint8_t initial_bit_read = SPI.transfer(0);
        Serial.print("Initial writing: 0x");
        Serial.println(initial_bit_read, HEX);
        delay(10);
        for (int i = 10; i <= 40; i += 10)
        {
            uint8_t bit_read = SPI.transfer(i);
            Serial.print("Writing: ");
            Serial.println(bit_read, DEC);
            delay(10); // 10ms
        }
        end_spi();
    }

    delay(1000);

    {
        begin_spi();
        SPI.transfer(CMD_READ);
        delay(10);
        uint8_t initial_bit_read = SPI.transfer(0);
        Serial.print("Initial reading: 0x");
        Serial.println(initial_bit_read, HEX);
        delay(10);
        for (int i = 10; i <= 40; i += 10)
        {
            uint8_t bit_read = SPI.transfer(0);
            Serial.print("Reading: ");
            Serial.println(bit_read, DEC);
            delay(10); // 10ms
        }
        end_spi();
    }

    delay(5000); // 5s
}

void setup()
{
    pinMode(FPGA_RESET_PIN, OUTPUT);
    pinMode(SPI_CS_PIN, OUTPUT);
    pinMode(SPI_CLK_PIN, OUTPUT);
    digitalWrite(FPGA_RESET_PIN, LOW);
    digitalWrite(SPI_CS_PIN, HIGH);
    digitalWrite(SPI_CLK_PIN, LOW);

    // Initial delay to ensure both devices are connected
    delay(5000);

    // Reset the FPGA
    digitalWrite(FPGA_RESET_PIN, HIGH);
    digitalWrite(SPI_CLK_PIN, HIGH);
    delay(1); /* enough for any SPI_SPEED greater than 1kHz */
    digitalWrite(SPI_CLK_PIN, LOW);
    digitalWrite(FPGA_RESET_PIN, LOW);

    Serial.begin(9600);
    SPI.begin();
    delay(1000);
}

void loop()
{
    test_set_get();
}
