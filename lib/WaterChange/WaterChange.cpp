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
    if (isActive())
    {
        logger.log("Program is already running...");
        return;
    }

    logger.log("Starting water change...");
    activate();

    if (_state == Events::WATER_LOW)
    {
        pour();
    }
    else
    {
        pumpOut();
    }
}

void Programs::WaterChange::stop()
{
    _pomp->turnOff();
    _water->turnOff();     
}

void Programs::WaterChange::pumpOut()
{
    logger.log("Pomping water out");
    _water->turnOff();
    _pomp->turnOn();
}

void Programs::WaterChange::pour()
{
    logger.log("Pouring water");
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

    if (event == Events::WATER_HIGH)
    {
        logger.log("Water high!");

        deactivate();
        stop();
    }
    else if (event == Events::WATER_LOW)
    {
        logger.log("Water low!");
        pour();
    }
    else if (event == Events::READING_ERROR)
    {
        logger.log("Reading error, terminating");
        stop();
    }
    _state = event;
}