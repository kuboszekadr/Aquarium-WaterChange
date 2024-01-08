#include "Sensors.h"

uint8_t Sensors::sensors_amount;                  // how many sensors are initalized
Sensors::Sensor *Sensors::sensors[SENSOR_AMOUNT]; // array of generated sensors
StaticJsonDocument<1024> Sensors::readings;

void Sensors::loop()
{
    // loop through sensors
    for (int i = 0; i < sensors_amount; i++)
    {
        Sensor *sensor = sensors[i];

        // check if sensor is reading for data collection
        if (sensor->isReady())
        {
            sensor->makeReading();
        }

        // check if sensor has collected enough data to share
        if (sensor->isAvailable())
        {
            if (!readings.size() == 0)
            {   
                readings.createNestedArray();
            }
            
            JsonObject reading = readings.createNestedObject();
            sensor->toJSON(reading); 
            
            sensor->checkTrigger();
            sensor->restart();
        }
    }    
}
