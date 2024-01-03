#ifndef ServiceWaterManager_h
#define ServiceWaterManager_h

#include "../WaterManager/WaterManager.h"

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
            static void post(AsyncWebServerRequest *request);
    };
}

#endif