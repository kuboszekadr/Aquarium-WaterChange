#ifndef WebServer__h
#define WebServer__h

#include "../Config/Config.h"
#include "../Logger/Logger.h"
#include "../Relay/Relay.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <SPIFFS.h>
#include <ESP32Time.h>
#include <Update.h>

namespace ESP32WebServer
{
    extern AsyncWebServer server;
    extern Logger logger;

    void start();
    void handle_Restart(AsyncWebServerRequest *request);

    void handle_GetConfigRequest(AsyncWebServerRequest *request);
    void handle_PostConfigRequest(AsyncWebServerRequest *request, JsonVariant &json);
    
    void handle_PostChangeRelayMode(AsyncWebServerRequest *request);
    void handle_GetRelayMode(AsyncWebServerRequest *request);

    void handle_GetSystemTime(AsyncWebServerRequest *request);
    void handle_GetReadings(AsyncWebServerRequest *request);

    void handle_FirmareUpdateOnRequest(AsyncWebServerRequest *request);
    void handle_FirmareUpdateOnUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

    void add_upload();

    extern StaticJsonDocument<256> last_reading;
}


#endif