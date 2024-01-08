#include "Config.h"


Config::Config(const char *name, const char *root)
{
    memcpy(_name, name, 15);
    sprintf(_path, "/%s/%s.json", root, name);
}

config_status_t Config::load()
{
    logger.logf("Opening: %s", _path);
    File config_file = SPIFFS.open(_path, FILE_READ);

    if (!config_file)
    {
        logger.logf("Config file %s does not exists!", _path);
        return CONFIG_FILE_ERROR;
    }

    logger.log("Deserializing...");
    DeserializationError err = deserializeJson(data, config_file);
    if (err)
    {
        logger.logf("Serialization error for %s...", _path);
        return CONFIG_SERIALIZATION_ERROR;
    }

    // logger.log("Config loaded sucesfully.");
    return CONFIG_LOADED;
}

config_status_t Config::save()
{
    File config_file = SPIFFS.open(_path, FILE_WRITE);
    if (!config_file)
    {
        return CONFIG_FILE_ERROR;
    }

    serializeJson(data, config_file);
    return CONFIG_SAVED;
}
