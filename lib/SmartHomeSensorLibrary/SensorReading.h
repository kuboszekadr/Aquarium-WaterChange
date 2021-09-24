#ifndef Reading_h
#define Reading_h

#include "Measures.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP32Time.h>

namespace Sensors
{
    class SensorReading : public ESP32Time
    {
    public:
        SensorReading(
            uint8_t sensor_id,
            Measures *measures_id,
            uint8_t measures_amount);
        void toJSON(JsonObject &doc);

    protected:
        float *_last_readings;

        uint8_t _sensor_id;
        uint8_t *_measures_id;
        uint8_t _measures_amount;

        float *_readings;
        uint8_t _readings_count;
    };
}

#endif