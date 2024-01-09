#include "WaterLevel.h"

Sensors::Measures Sensors::water_level_measure[1] = {Sensors::Measures::WATER_LEVEL};

Sensors::WaterLevel::WaterLevel(
    uint8_t echo,
    uint8_t trig,
    const char *name)
    : Sensor(
          water_level_measure,
          1,
          name,

          Events::EventType::WATER_LOW,
          Events::EventType::WATER_HIGH)
{
    _echo = echo; // echo pin
    _trig = trig; // trig pin
}

void Sensors::WaterLevel::start()
{
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
}

float Sensors::WaterLevel::makeReading()
{
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);

    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);

    digitalWrite(_trig, LOW);

    float reading = pulseIn(_echo, HIGH) / 58.0;
    _readings[0] += reading; // returns water level in cm
    _readings_count++;

    _last_reading = millis();
    return reading;
}

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

Events::EventType Sensors::WaterLevel::constantLevelHandler(float reading)
{
    if (reading >= _trigger_value_high)
    {
        return Events::EventType::WATER_LOW;
    }
    return Events::EventType::WATER_HIGH;
}