#include "Relay.h"

std::map <const char*, Relay*> Relay::relays;

Relay::Relay(const char *name, int pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH); // on ESP3s2 seems like HIGH=LOW on arduino...

    relays[name] = this;
}

Relay::~Relay()
{
    for (auto it = relays.begin(); it != relays.end(); ++it)
    {
        if (it->second == this)
        {
            relays.erase(it);
            break;
        }
    }
}

void Relay::turnOn()
{
    digitalWrite(_pin, LOW);
}

void Relay::turnOff()
{
    digitalWrite(_pin, HIGH);
}

int Relay::getState()
{
    return digitalRead(_pin);
}