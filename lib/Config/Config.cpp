#include "Config.h"

Config::Config(char *name)
{
    memcpy(_name, name, 15);
}

Config *Config::getByName(const char *configName)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        Config *config = configs[i];
        if (strcmp(config->name(), configName) == 0)
        {
            return config;
        }
    }
    return nullptr;
}