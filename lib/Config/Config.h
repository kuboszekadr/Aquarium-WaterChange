#ifndef Config__h
#define Config__h

#define CONFIG_FILES_AMOUNT 5

#include "../Logger/Logger.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ARDUINO
    #include <SPIFFS.h>
#endif


typedef enum
{
    CONFIG_LOADED,
    CONFIG_FILE_ERROR,
    CONFIG_FILE_DOESNT_EXIST,
    CONFIG_SERIALIZATION_ERROR,
    CONFIG_SAVED
} config_status_t;

class Config
{
public:
    Config(const char *name) : Config(name, "config") {};
    Config(const char *name, const char *root);

    void setPath(const char *path);
    char *name(){return _name;};

    virtual config_status_t load();
    virtual config_status_t save();

    JsonDocument data;

protected:
    char _name[16];
    char _path[33];

    Logger logger = Logger("config");
};

#endif