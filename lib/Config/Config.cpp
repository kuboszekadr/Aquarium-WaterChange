#include "Config.h"

StaticJsonDocument<256> Config::data;

config_status_t Config::load()
{
    File config_file = SPIFFS.open("config/wifi.json", "r");
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

void Config::save()
{
    // TODO
}
