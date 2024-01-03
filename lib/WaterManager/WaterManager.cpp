#include "WaterManager.h"

// Programs::WaterManager Programs::water_change = Programs::WaterManager(5, 6, 1); 

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
    logger.log("Refilling water");
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

    if (_keep_water_level)
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
        stop();
    }
    pour();

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
        logger.log("Tank refilled");
        stop();
    }
    else if (event == Events::WATER_LOW)
    {
        logger.log("Refilling...");
        pour();
    }
}

void Programs::WaterManager::configure(uint8_t pin_pomp, uint8_t pin_water)
{
    logger.log("Changing device configuration...");

    delete _pomp;
    delete _water;

    _pomp = new Relay("Pomp", pin_pomp);
    _water = new Relay("WaterIn", pin_water);

    logger.log("Device config updated");
}

void Programs::WaterManager::changeMode(bool keep_water_level)
{
    if (keep_water_level == _keep_water_level)
    {
        return;
    }
    
    _keep_water_level = keep_water_level;
    if (_keep_water_level)
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
    _keep_water_level = config.data["constant_level"];

    logger.logf(
        "Constant level mode: %s", 
        _keep_water_level ? "enabled" : "disabled"
    );
}

void Programs::WaterManager::saveConfig()
{
    logger.log("Save config...");

    Config config = Config("water_manager");
    config.data["constant_level"] = _keep_water_level;
    config.save();

    logger.log("Config saved");
}