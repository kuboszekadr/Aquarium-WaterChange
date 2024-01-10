#ifndef Reading_h
#define Reading_h

#include "Measures.h"

#include <ArduinoJson.h>
#include <ESP32Time.h>

#include <map>
#include <vector>
#include <string>
#include <utility>

namespace Sensors
{
    class Readings
    {
    public:
        Readings(std::vector<std::string> measures);

        void toJSON(JsonObject &doc);
        void restart();

        void addNewReading(std::string measure_name, float reading);
    private:
        std::map<std::string, std::vector<float>> _readings;
        uint8_t _readings_count;
    };
}

#endif