#include "SensorReading.h"

Sensors::SensorReading::SensorReading(uint8_t sensor_id, Measures measures_id, uint8_t measures_amount) : ESP32Time()
{
    _sensor_id = sensor_id;

    _measures_id = new uint8_t[_measures_amount]();
    for (uint8_t i = 0; i < measures_amount; i++)
    {
        // convert measures enum into uint8_t
        _measures_id[i] = static_cast<uint8_t>(_measures_id[i]);
    }
}

void Sensors::SensorReading::toJSON(JsonDocument &doc)
{
    doc.clear();
    doc["sensor_id"] = _sensor_id;
    doc["timestamp"] = getTime("%Y%m%d %H%M%S");

    JsonArray readings = doc.createNestedArray("readings");

    uint8_t i = 0;
    while (_measures_id[i])
    {
        JsonObject reading = readings.createNestedObject();
        reading["measure_id"] = _measures_id[i];
        reading["value"] = _readings[i];
        i++;
    }
}