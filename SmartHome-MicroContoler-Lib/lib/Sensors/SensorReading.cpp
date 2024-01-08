#include "SensorReading.h"

Sensors::SensorReading::SensorReading(
    uint8_t sensor_id,
    Measures *measures_id,
    uint8_t measures_amount) : ESP32Time()
{
    _sensor_id = sensor_id;

    _measures_amount = measures_amount;
    _measures_id = new uint8_t[_measures_amount]();
    _readings = new float[_measures_amount]();
    _last_readings = new float[_measures_amount]();

    for (uint8_t i = 0; i < measures_amount; i++)
    {
        // convert measures enum into uint8_t
        _measures_id[i] = static_cast<uint8_t>(measures_id[i]);
    }
}

void Sensors::SensorReading::toJSON(JsonObject &doc)
{
    String ts = getTime("%Y%m%d %H%M%S");

    for (uint8_t i = 0; i < _measures_amount; i++)
    {
        doc["measure_name"] = _measures_id[i];

        float value = _readings[i] / _readings_count;
        doc["value"] = value;
        _last_readings[i] = value;

        doc["timestamp"] = ts;
    }
}