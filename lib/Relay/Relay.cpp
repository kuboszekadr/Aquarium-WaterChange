#include "Relay.h"

Relay::Relay(int pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH); // on ESP32 seems like HIGH=LOW on arduino...
}

void Relay::turnOn()
{
    Serial.printf("Opening relay on pin %d\n", _pin);
    digitalWrite(_pin, LOW);
}

void Relay::turnOff()
{
    Serial.printf("Closing relay on pin %d\n", _pin);
    digitalWrite(_pin, HIGH);
}

int Relay::pin()
{
    return _pin;
}