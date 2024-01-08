#include "WaterManager.h"

Programs::WaterManager::WaterManager(uint8_t pin_pomp, uint8_t pin_water)
{
    // _pomp = new Relay("Pomp", pin_pomp);
    // _water = new Relay("WaterIn", pin_water);

    listenTo(Events::WATER_LOW);
    listenTo(Events::WATER_HIGH);
    listenTo(Events::READING_ERROR);

    loadConfig();
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
    // _pomp->turnOff();
    // _water->turnOff();     

    deactivate();
    _state = IDLE;
}

void Programs::WaterManager::pumpOut()
{
    // _water->turnOff();
    // _pomp->turnOn();
    
    _state = POMPING;
}

void Programs::WaterManager::pour()
{
    // _pomp->turnOff();
    // _water->turnOn(); 

    _state = POURING;
}

void Programs::WaterManager::reactForEvent(Events::EventType event)
{
    if (event == Events::READING_ERROR)
    {
        stop();
        return;
    }

    if (_keep_water_level)
    {
        constantLevelHandler(event);
    }
    else
    {
        defaultHandler(event);
    }

    _event = event;
}

void Programs::WaterManager::constantLevelHandler(Events::EventType event)
{
    // ignore the same state
    if (_event == event)
    {
        return;
    }

    if (event == Events::WATER_HIGH)
    {
        stop();
    }
    pour();

    _event = event;
}

void Programs::WaterManager::defaultHandler(Events::EventType event)
{
    // ignore the same state
    if (_event == event)
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
    // delete _pomp;
    // delete _water;

    // _pomp = new Relay("Pomp", pin_pomp);
    // _water = new Relay("WaterIn", pin_water);
}

void Programs::WaterManager::changeMode(bool keep_water_level)
{
    if (keep_water_level == _keep_water_level)
    {
        return;
    }
    
    _keep_water_level = keep_water_level;
    saveConfig();
}

void Programs::WaterManager::loadConfig()
{
    Config config = Config("water_manager");
    config.load();
    _keep_water_level = config.data["constant_level"];

    logger.logf(
        "Constant level mode: %s", 
        _keep_water_level ? "enabled" : "disabled"
    );
}

void Programs::WaterManager::saveConfig()
{
    Config config = Config("water_manager");
    config.data["constant_level"] = _keep_water_level;
    config.save();
}