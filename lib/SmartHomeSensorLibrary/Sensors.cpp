#include "Sensors.h"

void Sensors::loop()
{
    // loop through sensors
    for (int i = 0; i < sensors_amount; i++)
    {
        Sensor *sensor = sensors[i];
        char sensor_name[SENSOR_NAME_LENGHT + 1] = {};
        sensor->getName(sensor_name);

        // check if sensor is reading for data collection
        if (sensor->isReady())
        {
            sensor->makeReading();
        }

        // check if sensor has collected enough data to share
        if (sensor->isAvailable())
        {
            Events::EventType event = sensor->checkTrigger();
        }
    }
}