#include "Config.h"

Config::Config(const char *name)
{
    memcpy(_name, name, 15);
}

Config *Config::getByName(const char *configName)
{
    for (uint8_t i = 0; i < _files_amount; i++)
    {
        Config *config = _configs[i];
        if (strcmp(config->name(), configName) == 0)
        {
            return config;
        }
    }
    return nullptr;
}

config_status_t Config::load(const char *name)
{
    Config *config = Config::getByName(name);
    if (config == nullptr)
    {
        /* code */
    }
        


        

    return CONFIG_LOADED;
}

void Config::loadConfigFile()
{
    char file_path[32];
    strcpy(file_path, "/config/");
    strcat(file_path, _name);
    strcat(file_path, ".json");

    File config_file = SPIFFS.open(file_path);
    if (!config_file)
    {
        return;
    }

    DeserializationError err = deserializeJson(data, config_file);
    if (err)
    {
        return;
    }

}

void Config::_loadNewConfig(const char *name)
{
    Config *config = new Config(name);
    _configs[_files_amount] = config;
    _files_amount++;
}