#ifndef WebServer__h
#define WebServer__h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

namespace ESP32WebServer
{
    extern AsyncWebServer server;
    void start();
    void handle_GetConfigRequest(AsyncWebServerRequest *request);
}

#endif