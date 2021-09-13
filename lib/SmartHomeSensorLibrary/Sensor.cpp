#include "Sensor.h"

Sensors::Sensor::Sensor(
    uint8_t sensor_id,
    Measures *measures_id,
    uint8_t measures_amount,
    const char *name,

    uint32_t sampling_interval,
    uint8_t sampling_amount,

    float trigger_value_low,
    float trigger_value_high,

    Events::EventType trigger_low,
    Events::EventType trigger_high) : SensorReading(
        sensor_id,
        measures_id,
        measures_amount
    )
{
    if (sensors_amount == SENSOR_AMOUNT)
    {
        return;
    }

    sensors[sensors_amount] = this;                            // add sensor to the list of sensors
    sensors_amount += sensors_amount == SENSOR_AMOUNT ? 0 : 1; // increase amount of sensors

    _readings = new float[_measures_amount]();
    _last_readings = new float[_measures_amount]();

    _name = name;

    // setup trigger values and trigers
    _trigger_value_low = trigger_value_low;
    _trigger_low = trigger_low;

    _trigger_value_high = trigger_value_high;
    _trigger_high = trigger_high;

    _sampling_amount = sampling_amount;
    _sampling_interval = sampling_interval;
}

Events::EventType Sensors::Sensor::checkTrigger()
{
    Events::EventType event = Events::EventType::EMPTY;
    if (_last_readings[0] < _trigger_value_low)
    {
        event = _trigger_low;
    }
    else if (_last_readings[0] > _trigger_value_high)
    {
        event = _trigger_high;
    }
    return event;
}

void Sensors::Sensor::getName(char *buffer)
{
    strncpy(buffer, _name, SENSOR_NAME_LENGHT);
}
