#include "Sensor.h"

Sensors::Sensor::Sensor(
    Measures *measures_id,
    uint8_t measures_amount,
    const char *name,
    Events::EventType trigger_low,
    Events::EventType trigger_high)
{
    if (sensors_amount == SENSOR_AMOUNT)
    {
        return;
    }

    sensors[sensors_amount] = this; // add sensor to the list of sensors
    sensors_amount++;

    _trigger_low = trigger_low;
    _trigger_high = trigger_high;

    // _readings = Readings(measures_id, measures_amount);
    _name = name;
}

void Sensors::Sensor::getName(char *buffer)
{
    strncpy(buffer, _name, SENSOR_NAME_LENGHT);
}

void Sensors::Sensor::setTriggerValues(float low, float high)
{
    _trigger_value_high = high;
    _trigger_value_low = low;
}

void Sensors::Sensor::setSampling(uint8_t amount, uint32_t interval)
{
    _sampling_amount = amount;
    _sampling_interval = interval;
}
