#ifndef WebServer__h
#define WebServer__h

#include "Config.h"
#include "Relay.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <Logger.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Time.h>

namespace ESP32WebServer
{
    extern AsyncWebServer server;
    extern Logger logger;
    void start();
    
    void handle_GetConfigRequest(AsyncWebServerRequest *request);
    void handle_PostConfigRequest(AsyncWebServerRequest *request, JsonVariant &json);
    
    void handle_PostChangeRelayMode(AsyncWebServerRequest *request);
    void handle_GetRelayMode(AsyncWebServerRequest *request);

    void handle_GetSystemTime(AsyncWebServerRequest *request);
    void handle_GetReadings(AsyncWebServerRequest *request);

    extern StaticJsonDocument<256> last_reading;
}


#endif