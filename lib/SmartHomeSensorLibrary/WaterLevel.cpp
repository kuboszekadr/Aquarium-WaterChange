#include "WaterLevel.h"

Sensors::Measures Sensors::water_level_measure[1] = {Sensors::Measures::WATER_LEVEL};

Sensors::WaterLevel::WaterLevel(
    uint8_t echo,
    uint8_t trig,
    uint8_t id_sensor,
    const char *name,

    float trigger_value_low,
    float trigger_value_high,

    uint32_t sampling_interval,
    uint8_t sampling_amount)
    : Sensor(
          id_sensor,
          water_level_measure,
          1,
          name,

          sampling_interval,
          sampling_amount,

          trigger_value_low,
          trigger_value_high,

          Events::EventType::WATER_LOW,
          Events::EventType::WATER_HIGH)
{
    _echo = echo; // echo pin
    _trig = trig; // trig pin

    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
}

bool Sensors::WaterLevel::makeReading()
{

    digitalWrite(_trig, LOW);
    delayMicroseconds(2);

    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);

    // Measuring distance
    digitalWrite(_trig, LOW);

    // Get value
    float reading = pulseIn(_echo, HIGH) / 58.0;
    _readings[0] += reading; // returns water level in cm
    _readings_count++;

    _last_reading = millis();
    return true;
}

Events::EventType Sensors::WaterLevel::checkTrigger()
{
    Events::EventType event = Events::EventType::EMPTY;
    if (!_readings_count)
    {
        return event;
    }
    
    // check current level of water
    if (_last_readings[0] > _trigger_value_low)
    {
        event = _trigger_low;
    }
    else if (_last_readings[0] < _trigger_value_high)
    {
        event = _trigger_high;
    }

    // push to the queue if event is not empty
    if ((event != Events::EventType::EMPTY) & (event != _last_trigger))
    {
        Events::raise(event);
    }

    return event;
}