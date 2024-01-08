#ifndef LoggerAPI__h
#define LoggerAPI__h

#include <ArduinoJson.h>
#include <Device/Device.h>

namespace Loggers
{
    void logToAPI(const char *module_name,
             const char *log_level,
             const char *msg,
             const char *timestamp)
    {
        StaticJsonDocument<512> doc;
        JsonObject obj = doc.to<JsonObject>();

        obj["device_name"] = Device::device->device_name;
        obj["module_name"] = module_name;
        obj["log_level"] = log_level;
        obj["msg"] = msg;
        obj["log_timestamp"] = timestamp;

        Device::device->postLog(obj);
    }
}

#endif