#ifndef Config__h
#define Config__h

#define CONFIG_FILES_AMOUNT 3

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
} config_status_t;

class Config
{
public:
    Config(const char *name);

    char *name(){return _name;};

    config_status_t load();
    config_status_t save(JsonObject &data);

    StaticJsonDocument<128> data;

    static config_status_t load(const char *name);
    static config_status_t save(const char *name);
    static Config *getByName(const char *name);

protected:
    char _name[16];
    void loadConfigFile();

    static Config *_configs[CONFIG_FILES_AMOUNT];
    static uint8_t _files_amount;
    static void _loadNewConfig(const char *name);
};

#endif