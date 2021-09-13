#ifndef Sensor_h
#define Sensor_h

#include "EventType.h"
#include "Measures.h"
#include "SensorReading.h"

#include <Arduino.h>

#define SENSOR_AMOUNT 10 // maximum amount of sensors
#define SENSOR_NAME_LENGHT 20

namespace Sensors
{
    class Sensor : public SensorReading
    {
    public:
        Sensor(uint8_t sensor_id,
               Measures *measures_id,
               uint8_t measures_amount,
               const char *name,

               uint32_t sampling_interval,
               uint8_t sampling_amount,

               float trigger_value_low,
               float trigger_value_high,

               Events::EventType trigger_low,
               Events::EventType trigger_high);

        virtual bool makeReading() = 0;           // to be overwriten by the subclasses
        virtual Events::EventType checkTrigger(); // check if current level of sensor value is between low and high trigger

        bool isAvailable() { return _readings_count >= _sampling_amount; };          // check if sensor gathered enough data
        bool isReady() { return (millis() - _last_reading) >= _sampling_interval; }; // check if sensor can gather data

        void getName(char *buffer);
        float *getReadings() { return _last_readings; };

    protected:
        float *_last_readings;       // to store
        float *_readings;            // array to hold all readings done before publishing
        uint8_t _readings_count = 0; // amount of readings done in the sesion

        uint32_t _sampling_interval;
        uint8_t _sampling_amount;

        uint32_t _last_reading = 0L; // when last reading was done (as millis)

        float _trigger_value_low = -1.0; // average sensor value
        Events::EventType _trigger_low;  // event to be rised when sensor value is below low value

        float _trigger_value_high = -1.0; // +/- level when specify event should be published
        Events::EventType _trigger_high;  // event to be rised when sensor value is above value

        Events::EventType _last_trigger = Events::EventType::EMPTY;

        const char *_name; // sensor name / label
    };                     // namespace Sensors

    extern Sensor *sensors[SENSOR_AMOUNT]; // array of generated sensors
    extern uint8_t sensors_amount;         // how many sensors are initalized
}

#endif