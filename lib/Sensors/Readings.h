#ifndef Reading_h
#define Reading_h

#include "Measures.h"

#include <ArduinoJson.h>
// #include <ESP32Time.h>

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
    
        std::map<std::string, std::vector<float>> getReadings() { return _readings; }
    
    private:
        std::map<std::string, std::vector<float>> _readings;
    };
}

#endif