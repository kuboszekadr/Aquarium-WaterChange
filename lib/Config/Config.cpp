#include "Config.h"

StaticJsonDocument<256> Config::data;

config_status_t Config::load()
{
    File config_file = SPIFFS.open("/config/wifi.json", "r");
    if (!config_file)
    {
        return CONFIG_FILE_ERROR;
    }

    DeserializationError err = deserializeJson(Config::data, config_file);
    if (err)
    {
        return CONFIG_SERIALIZATION_ERROR;
    }

    return CONFIG_LOADED;
}

config_status_t Config::save()
{
    File config_file = SPIFFS.open("/config/wifi.json", "w");
    if (!config_file)
    {
        return CONFIG_FILE_ERROR;
    }

    serializeJson(data, config_file);
    return CONFIG_SAVED;
}
