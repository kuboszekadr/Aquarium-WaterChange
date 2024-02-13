#include "WaterLevel.h"

Sensors::WaterLevel::WaterLevel(
    uint8_t echo,
    uint8_t trig,
    const char *name)
    : Sensor(
          {std::string("WaterLevel")},
          name,

          Events::EventType::WATER_LOW,
          Events::EventType::WATER_HIGH)
{
    _echo = echo; // echo pin
    _trig = trig; // trig pin

    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);   
}

/**
 * Makes a reading of the water level using the ultrasonic sensor.
 * 
 * @return The measured water level in centimeters.
 */
float Sensors::WaterLevel::makeReading()
{
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);

    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);

    digitalWrite(_trig, LOW);

    float reading = pulseIn(_echo, HIGH) / 58.0;

    _readings.addNewReading("WaterLevel", reading);
    
    _last_reading = millis();
    _readings_amount++;
    return reading;
}

/**
 * @brief Checks the trigger based on the water level reading.
 * 
 * This function delegates the trigger checking based on current state of the constant water level monitoring.
 * 
 * @param reading The water level reading.
 * @return The event type based on the reading.
 */
Events::EventType Sensors::WaterLevel::checkTrigger(float reading)
{
    Events::EventType event = Events::EventType::EMPTY;
    if (reading <= 0.0 || reading >= 99.0)
    {
        return Events::EventType::READING_ERROR;
    }

    // check if water level is constant
    if (keep_water_level)
    {
        event = constantLevelHandler(reading);
    }
    else
    {
        event = defaultHandler(reading);
    }

    // push to the queue if event is not empty
    if (event != Events::EventType::EMPTY)
    {
        Events::raise(event);
    }

    return event;
}

/**
 * @brief Default handler for the water level sensor reading while constant level is turned off.
 * 
 * @param reading The water level sensor reading.
 * @return The event type based on the water level reading.
 */
Events::EventType Sensors::WaterLevel::defaultHandler(float reading)
{
    if (reading >= _trigger_value_low)
    {
        return Events::EventType::WATER_LOW;
    }
    else if (reading <= _trigger_value_high)
    {
        return Events::EventType::WATER_HIGH;
    }
    return Events::EventType::EMPTY;
}

/**
 * @brief Default handler for the water level sensor reading while constant level is turned on.
 * 
 * @param reading The water level sensor reading.
 * @return The event type based on the water level reading.
 */
Events::EventType Sensors::WaterLevel::constantLevelHandler(float reading)
{
    if (reading >= _trigger_value_high)
    {
        return Events::EventType::WATER_LOW;
    }
    return Events::EventType::WATER_HIGH;
}

/**
 * @brief Serializes the sensor data to JSON.
 * 
 * This function serializes the sensor data to JSON and sends it to the serial output.
 */ 
void Sensors::WaterLevel::saveConfig()
{
    Config config = Config("sensor");

    JsonDocument& doc = config.data;    
    doc["keep_water_level"] = keep_water_level;
    
    doc["trigger_low"] = _trigger_value_low;
    doc["trigger_high"] = _trigger_value_high;

    doc["sampling_amount"] = _sampling_amount;
    doc["sampling_interval"] = _sampling_interval;

    serializeJsonPretty(doc, Serial);

    config.save();
}

/**
 * @brief Loads the sensor configuration from the configuration file.
 * 
 * This function loads the sensor configuration from the configuration file.
 */
void Sensors::WaterLevel::loadConfig()
{
    Config config = Config("sensor");
    config.load();

    keep_water_level = config.data["keep_water_level"] | false;

    _trigger_value_low = config.data["trigger_low"];
    _trigger_value_high = config.data["trigger_high"];

    _sampling_amount = config.data["sampling_amount"] | 30;
    _sampling_interval = config.data["sampling_interval"] | 2000;
}