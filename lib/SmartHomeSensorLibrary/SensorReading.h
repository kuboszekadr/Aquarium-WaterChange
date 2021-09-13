#ifndef Reading_h
#define Reading_h

#include "Measures.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP32Time.h>

#define ARRAY_COUNT(arr) sizeof(arr)/sizeof(arr[0])

namespace Sensors
{
    class SensorReading : public ESP32Time
    {
    public:
        SensorReading(
            uint8_t sensor_id,
            Measures *measures_id
            );
        void toJSON(JsonDocument &doc);
    
    protected:
        uint8_t _sensor_id;
        uint8_t *_measures_id;
        uint8_t _measures_amount;

        float *_readings;
    };
}

#endif