#include "Sensors.h"

uint8_t Sensors::sensors_amount;                  // how many sensors are initalized
Sensors::Sensor *Sensors::sensors[SENSOR_AMOUNT]; // array of generated sensors
JsonDocument Sensors::readings;

void Sensors::loop()
{
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
            if (!readings.size() == 0)
            {   
                readings.to<JsonArray>();
            }
            
            JsonArray records = sensor->getReadings().toJSON().as<JsonArray>();
            for (JsonVariant record : records)
            {
                readings.add(record);
            }
            
            //FIXME: simplification 
            sensor->checkTrigger(records[0]["value"].as<float>());
            sensor->restart();
        }
    }    
}
