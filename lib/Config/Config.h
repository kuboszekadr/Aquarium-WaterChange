#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>

namespace Config
{
    void load();
    void save();

    extern StaticJsonDocument<256> data;
}