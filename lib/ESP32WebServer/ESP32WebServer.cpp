#include "ESP32WebServer.h"

AsyncWebServer ESP32WebServer::server(80);

void ESP32WebServer::start()
{
    ESP32WebServer::server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                              { request->send(SPIFFS, "/index.html", "text/html"); });
    ESP32WebServer::server.on("/config", HTTP_GET, handle_GetConfigRequest);

    ESP32WebServer::server.on("/src/css/bootstrap.css", HTTP_GET, [](AsyncWebServerRequest *request)
                              { request->send(SPIFFS, "/src/css/bootstrap.css", "text/css"); });
    ESP32WebServer::server.on("/src/css/custom.min.css", HTTP_GET, [](AsyncWebServerRequest *request)
                              { request->send(SPIFFS, "/src/css/custom.min.css", "text/css"); });
    ESP32WebServer::server.on("/src/js/jquery.min", HTTP_GET, [](AsyncWebServerRequest *request)
                              { request->send(SPIFFS, "/src/js/jquery.min", "text/javascript"); });
    ESP32WebServer::server.begin();

    Serial.println("Server started on port 80");
};

void ESP32WebServer::handle_GetConfigRequest(AsyncWebServerRequest *request)
{
    Serial.print(F("New request from IP: "));
    Serial.println(request->client()->remoteIP());

    if (Config::load() != CONFIG_LOADED)
    {
        request->send(
            500,
            "application/json",
            "Error during config loading");
    };

    char config[100] = "";
    serializeJson(Config::data, config);

    request->send(
        200,
        "application/json",
        config);
}
