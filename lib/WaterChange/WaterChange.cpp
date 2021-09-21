#include "WaterChange.h"

Programs::WaterChange::WaterChange(uint8_t pin_pomp, uint8_t pin_water, uint8_t id)
{
    _pomp = new Relay(pin_pomp);
    _water = new Relay(pin_water);

    _id = id;

    listenTo(Events::WATER_LOW);
    listenTo(Events::WATER_HIGH);
}

void Programs::WaterChange::start()
{
    if (_is_active)
    {
        return;
    }

    Serial.println("Starting water change...");
    _is_active = true;

    // check latest water status, if low only pour
    if (_state == Events::WATER_LOW)
    {
        pour();
    }
    else // otherwise, start with water pumping out
    {
        pumpOut();
    }
}

void Programs::WaterChange::pumpOut()
{
    Serial.println("Pomping water out");
    _water->turnOff();
    _pomp->turnOn();
}

void Programs::WaterChange::pour()
{
    Serial.println("Pouring water");
    _pomp->turnOff();
    _water->turnOn(); 
}

void Programs::WaterChange::reactForEvent(Events::EventType event)
{
    // ignore the same state
    if (_state == event)
    {
        return;
    }

    // Check if program can be closed
    if (event == Events::WATER_HIGH)
    {
        if (_is_active)
        {
            _is_active = false; // finish water change
        }
        
        // turn of the relays
        _water->turnOff(); 
        _pomp->turnOff();

        _state = event;
    }
    // check if water has to be poured
    else if (event == Events::WATER_LOW)
    {
        _state = event;
        pour();
    }
}