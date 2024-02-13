#ifndef ServiceWaterManager_h
#define ServiceWaterManager_h

#include "WaterLevel.h"
#include "Services/Services.h"

#include <ArduinoJson.h>
#include <AsyncJson.h>

namespace Services
{
    class ServiceWaterManager : public IService
    {
        public:
            void create();

            static void get(AsyncWebServerRequest *request);
            static void post(AsyncWebServerRequest *request, JsonVariant &json);
    };

    extern ServiceWaterManager water_manager_service;
}

extern Sensors::WaterLevel water_level_sensor;
extern bool constant_water_level_setup;

#endif