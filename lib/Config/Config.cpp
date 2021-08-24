#include "Config.h"

uint8_t Config::_files_amount = 0;
Config *Config::_configs[CONFIG_FILES_AMOUNT];

Config::Config(const char *name)
{
    memcpy(_name, name, 15);
    _configs[_files_amount] = this;
    _files_amount++;
}

Config *Config::getByName(const char *configName)
{
    Serial.printf("Getting %s config.\n", configName);
    Serial.println(_files_amount);
    for (uint8_t i = 0; i < _files_amount; i++)
    {
        Config *config = _configs[i];
        if (strcmp(config->name(), configName) == 0)
        {
            return config;
        }
    }
    Serial.printf("%s has not been found.", configName);
    return nullptr;
}

config_status_t Config::load(const char *name)
{
    Config *config = Config::getByName(name);
    if (config == nullptr)
    {
        Serial.printf("Config %s not found, creating new.\n", name);
        config = new Config(name);
    }

    Serial.println("Loading config file...");
    config->load();
    return CONFIG_LOADED;
}

config_status_t Config::load()
{
    char file_path[32];
    strcpy(file_path, "/config/");
    strcat(file_path, _name);
    strcat(file_path, ".json");

    Serial.print("Loading: ");
    Serial.println(file_path);

    File config_file = SPIFFS.open(file_path);
    if (!config_file)
    {
        return CONFIG_FILE_ERROR;
    }

    DeserializationError err = deserializeJson(data, config_file);
    if (err)
    {
        return CONFIG_SERIALIZATION_ERROR;
    }

    return CONFIG_LOADED;
}
