#include "WaterManager.h"

Programs::WaterManager::WaterManager(uint8_t pin_pomp, uint8_t pin_water, uint8_t id)
{
    _pomp = new Relay("Pomp", pin_pomp);
    _water = new Relay("WaterIn", pin_water);

    _id = id;

    listenTo(Events::WATER_LOW);
    listenTo(Events::WATER_HIGH);
    listenTo(Events::READING_ERROR);

    loadConfig();
}

void Programs::WaterManager::start()
{
    if (constant_level)
    {
        logger.log("Constant level mode enabled, please use changeMode() to disable it");
        return;
    }

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

void Programs::WaterManager::stop()
{
    logger.log("Stopping program execution...");
    _pomp->turnOff();
    _water->turnOff();     

    deactivate();
    Notification::push("WaterManager - water change task", "Task finished - relays closed");
}

void Programs::WaterManager::pumpOut()
{
    logger.log("Pomping water out");
    _water->turnOff();
    _pomp->turnOn();
    
    Notification::push("WaterManager - water change task", "Task pomping water out");
}

void Programs::WaterManager::pour()
{
    logger.log("Pouring water");
    _pomp->turnOff();
    _water->turnOn(); 

    Notification::push("WaterManager - water change task", "Filling water");
}

void Programs::WaterManager::reactForEvent(Events::EventType event)
{
    if (event == Events::READING_ERROR)
    {
        logger.log("Reading error, terminating");
        Notification::push("WaterManager - error", "Reading error please check device");
        stop();
        return;
    }

    if (constant_level)
    {
        constantLevelHandler(event);
    }
    else
    {
        defaultHandler(event);
    }

    _state = event;
}

void Programs::WaterManager::constantLevelHandler(Events::EventType event)
{
    // ignore the same state
    if (_state == event)
    {
        return;
    }

    if (event == Events::WATER_HIGH)
    {
        pour();
    }
    _state = event;
}

void Programs::WaterManager::defaultHandler(Events::EventType event)
{
    // ignore the same state
    if (_state == event)
    {
        return;
    }

    if (event == Events::WATER_HIGH)
    {
        logger.log("Water high!");
        stop();
    }
    else if (event == Events::WATER_LOW)
    {
        logger.log("Water low!");
        pour();
    }
}

void Programs::WaterManager::changeMode()
{
    constant_level = !constant_level;
    if (constant_level)
    {
        logger.log("Constant level mode enabled");
        Notification::push("WaterManager - constant level mode", "Constant level mode enabled");
    }
    else
    {
        logger.log("Constant level mode disabled");
        Notification::push("WaterManager - constant level mode", "Constant level mode disabled");
    }

    saveConfig();
}

void Programs::WaterManager::loadConfig()
{
    logger.log("Loading config...");

    Config config = Config("water_manager");
    config.load();
    constant_level = config.data["constant_level"];

    logger.logf(
        "Constant level mode: %s", 
        constant_level ? "enabled" : "disabled"
    );
}

void Programs::WaterManager::saveConfig()
{
    logger.log("Save config...");

    Config config = Config("water_manager");
    config.data["constant_level"] = constant_level;
    config.save();

    logger.log("Config saved");
}