#include <SPI.h>

#define SPI_SPEED 500000 // 500kHz
#define SPI_CLK_PIN 13
#define SPI_MISO_PIN 12
#define SPI_MOSI_PIN 11
#define SPI_CS_PIN 10
#define FPGA_RESET_PIN 9

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
    for (int i = 10; i <= 250; i += 10)
    {
        SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE0));

        digitalWrite(SPI_CS_PIN, LOW);
        uint8_t bit_read = SPI.transfer(i);
        digitalWrite(SPI_CS_PIN, HIGH);

        SPI.endTransaction();
        Serial.print("Bit read: ");
        Serial.println(bit_read, DEC);

        delay(10); // 10ms
    }

    delay(5000); // 5s
}
