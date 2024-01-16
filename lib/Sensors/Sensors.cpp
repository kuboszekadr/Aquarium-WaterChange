#include "Sensors.h"

uint8_t Sensors::sensors_amount;                  // how many sensors are initalized
Sensors::Sensor *Sensors::sensors[SENSOR_AMOUNT]; // array of generated sensors
JsonDocument Sensors::readings;

void Sensors::loop()
{
    if (readings.size() == 0)
    {   
        readings.to<JsonArray>();
    }
    
    // loop through sensors
    for (int i = 0; i < sensors_amount; i++)
    {
        Sensor *sensor = sensors[i];

        if (sensor->isReady())
        {
            sensor->makeReading();
        }

        if (sensor->isAvailable())
        {
            JsonDocument doc = sensor->getReadings().toJSON();
            JsonArray arr = doc.as<JsonArray>();

            for (JsonObject record : arr)
            {
                readings.add(record);
            }

            //FIXME: simplification 
            sensor->checkTrigger(arr[0]["value"].as<float>());
            sensor->restart();
        }
    }    
}
