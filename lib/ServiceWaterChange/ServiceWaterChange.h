#ifndef ServiceWaterChange_h
#define ServiceWaterChange_h

#include "WaterManager.h"
#include "Services/Services.h"

#include <ArduinoJson.h>
#include <AsyncJson.h>

namespace Services
{
    class ServiceWaterChange : public IService
    {
        public:
            void create();

            static void post(AsyncWebServerRequest *request, JsonVariant &json);
    };

    extern ServiceWaterChange water_change;
}

#endif