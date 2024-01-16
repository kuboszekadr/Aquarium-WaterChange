#include "WaterManager.h"

Programs::WaterManager::WaterManager(uint8_t pin_pomp, uint8_t pin_water)
{
    _pomp = new Relay("Pomp", pin_pomp);
    _water = new Relay("WaterIn", pin_water);

    listenTo(Events::WATER_LOW);
    listenTo(Events::WATER_HIGH);
    listenTo(Events::READING_ERROR);
}

void Programs::WaterManager::start()
{
    if (isActive())
    {
        return;
    }

    activate();

    if (_event == Events::WATER_LOW)
    {
        pour();
    }
    else
    {
        pumpOut();
    }
}

void Programs::WaterManager::stop()
{
    _pomp->turnOff();
    _water->turnOff();     

    deactivate();
    _state = IDLE;
}

void Programs::WaterManager::pumpOut()
{
    _water->turnOff();
    _pomp->turnOn();
    
    _state = POMPING;
}

void Programs::WaterManager::pour()
{
    _pomp->turnOff();
    _water->turnOn(); 

    _state = POURING;
}

void Programs::WaterManager::reactForEvent(Events::EventType event)
{
    if (event == Events::READING_ERROR)
    {
        stop();
        return;
    }

    // ignore the same state
    if (
        (_event == event) || 
        (_state == POMPING && event == Events::WATER_HIGH)
        )
    {
        return;
    }

    if (event == Events::WATER_HIGH)
    {
        stop();
    }
    else if (event == Events::WATER_LOW)
    {
        pour();
    }
}

void Programs::WaterManager::configure(uint8_t pin_pomp, uint8_t pin_water)
{
    delete _pomp;
    delete _water;

    _pomp = new Relay("Pomp", pin_pomp);
    _water = new Relay("WaterIn", pin_water);
}