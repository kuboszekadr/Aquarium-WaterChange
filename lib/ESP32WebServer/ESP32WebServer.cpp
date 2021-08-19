#include "ESP32WebServer.h"

AsyncWebServer ESP32WebServer::server(80);

void ESP32WebServer::start()
{
    ESP32WebServer::server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                              { request->send(SPIFFS, "/index.html", "text/html"); });
    ESP32WebServer::server.on("/config", HTTP_GET, ESP32WebServer::handle_GetConfigRequest);

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
    request->send(
        200,
        "application/json",
        R"=({"ssid":"Zdrajcy metalu","pwd":"Dz3nt31m3n_m3ta1u"})=");
}
