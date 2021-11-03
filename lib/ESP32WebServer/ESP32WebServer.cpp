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

    const char *arg = request->argName(0).c_str();
    Config *config = Config::getByName(arg);

    if (config == nullptr)
    {
        Config::load(arg);
        return;
    }

    char response[256] = "";
    serializeJson(config->data, response);
    serializeJson(config->data, Serial);

    request->send(
        200,
        "application/json",
        response);    

    delete arg;
}

void ESP32WebServer::handle_PostConfigRequest(AsyncWebServerRequest *request, JsonVariant &json)
{
    Serial.println("Config update requested - updating.");
    JsonObject obj = json.as<JsonObject>();

    const char *arg = request->argName(0).c_str();
    Config *config = Config::getByName(arg);

    config->data = obj;
    serializeJsonPretty(obj, Serial);

    int status = config->save();

    if (status != CONFIG_SAVED)
    {
        Serial.print("Error during config update: ");
        Serial.println(status);
        request->send(500);
    }
    
    Serial.println("File saved.");
    request->send(200);

    delete arg;
}
