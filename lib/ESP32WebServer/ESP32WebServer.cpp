#include "ESP32WebServer.h"

AsyncWebServer ESP32WebServer::server(80);

void ESP32WebServer::start()
{
    server.serveStatic("/", SPIFFS, "/www/");
    server.on("/config", HTTP_GET, handle_GetConfigRequest);

    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
        "/config",
        handle_PostConfigRequest);
    handler->setMethod(HTTP_POST);
    ESP32WebServer::server.begin();

    server.addHandler(handler);
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

void ESP32WebServer::handle_PostConfigRequest(AsyncWebServerRequest *request, JsonVariant &json)
{
    Serial.println("Config update requested - updating");

    JsonObject obj = json.as<JsonObject>();

    const char* ssid = obj["ssid"];
    const char* pwd = obj["pwd"];

    Config::data["ssid"] = ssid;
    Config::data["pwd"] = pwd;

    if (Config::save() == CONFIG_SAVED)
    {
        Serial.println("Config file updated.");
        request->send(200);
        
    }
    else 
    {
        request->send(500);
    }

}
