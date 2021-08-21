#ifndef Config__h
#define Config__h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

typedef enum
{
    CONFIG_LOADED,
    CONFIG_FILE_ERROR,
    CONFIG_SERIALIZATION_ERROR,
    CONFIG_SAVED
}
config_status_t;

namespace Config
{
    config_status_t load();
    config_status_t save();

    extern StaticJsonDocument<256> data;
}

#endif