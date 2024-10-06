#include <Arduino.h>

#define SPI_SS_PIN 10

void setup()
{
    pinMode(SPI_SS_PIN, OUTPUT);
}

void loop()
{
    while (true)
    {
        digitalWrite(SPI_SS_PIN, LOW);
        delay(500);
        digitalWrite(SPI_SS_PIN, HIGH);
        delay(500);
    }
}
