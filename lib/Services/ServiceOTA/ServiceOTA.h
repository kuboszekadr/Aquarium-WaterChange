#ifndef ServiceOTA__h
#define ServiceOTA__h

// this one in inspired by: https://github.com/ayushsharma82/AsyncElegantOTA/blob/master/src/AsyncElegantOTA.h
#include "../Services.h"

#include <Update.h>

namespace Services
{
    class ServiceOTA : public IService
    {
        public:
            void create();

            static void updateOnRequest(AsyncWebServerRequest *request);
            static void updateOnUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    };
}

#endif