#include "Readings.h"

Sensors::Readings::Readings(std::vector<std::string> measures)
{
    for (const auto &measure : measures) 
    {
        _readings.insert(
            std::make_pair(measure, std::vector<float>())
        );
    }
}

void Sensors::Readings::toJSON(JsonObject &doc)
{
    // ESP32Time time;
    // String ts = time.getTime("%Y%m%d %H%M%S");

    // for (uint8_t i = 0; i < _measures_amount; i++)
    // {
    //     doc["measure_name"] = _measures_id[i];

    //     float value = _readings[i] / _readings_count;
    //     doc["value"] = value;

    //     doc["timestamp"] = ts;
    // }
}

void Sensors::Readings::addNewReading(std::string measure_name, float reading)
{
    _readings[measure_name].push_back(reading);
}

void Sensors::Readings::restart()
{
    for (auto &reading : _readings)
    {
        reading.second.clear();
    }
}