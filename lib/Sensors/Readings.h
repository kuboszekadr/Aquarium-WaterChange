#ifndef Reading_h
#define Reading_h

#include "Measures.h"

#include <ArduinoJson.h>

#ifdef ARDUINO
    #include "../Timestamp/Timestamp.h"
#else
    #include <string>
#endif

#include <map>
#include <vector>
#include <utility>
#include <numeric>

namespace Sensors
{
    class Readings
    {
    public:
        Readings() {};
        ~Readings();
        Readings(std::vector<std::string> measures);

        JsonDocument  toJSON();
        void restart();
        void addNewReading(std::string measure_name, float reading);
    
        std::map<std::string, std::vector<float>> getReadings() { return _readings; }
    
    private:
        std::map<std::string, std::vector<float>> _readings;
    };
}

#endif