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

Sensors::Readings::~Readings()
{
    _readings.clear();
}

JsonDocument Sensors::Readings::toJSON()
{
#ifdef ARDUINO
    char *ts = getTimestamp();
#else
    const char ts[] = "20200101 000000";
#endif

    JsonDocument doc;
    JsonArray records = doc.to<JsonArray>();

    for (const auto &pair : _readings)
    {
        std::string measure_name = pair.first;
        std::vector<float> readings = pair.second;

        float sum = std::accumulate(readings.begin(), readings.end(), 0.0f);
        float average = readings.empty() ? 0.0f : sum / readings.size();

        JsonObject record = records.add<JsonObject>();
        record["measure_name"] = measure_name;
        record["value"] = average;
        record["timestamp"] = ts;
    }

    return doc;
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