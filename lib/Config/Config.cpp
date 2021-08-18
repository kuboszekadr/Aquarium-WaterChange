#ifndef Config__h
#define Config__h

#include "Config.h"

StaticJsonDocument<256> Config::data;

void Config::load()
{
    File config_file = SPIFFS.open("/config.json", "r");
    if (!config_file)
    {
        Serial.println("Failed to read config file.");
    }

    // const char* config = R"=({"ssid":"Zdrajcy metalu","pwd":"Dz3nt31m3n_m3ta1u"})=";

    DeserializationError err = deserializeJson(Config::data, config_file);
    if (err)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(err.c_str());
    }
}

void Config::save()
{
}

#endif