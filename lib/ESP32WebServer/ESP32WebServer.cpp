#include "ESP32WebServer.h"

AsyncWebServer ESP32WebServer::server(80);

void ESP32WebServer::start()
{
    server.serveStatic("/", SPIFFS, "/www/");
    server.on("/config", HTTP_GET, handle_GetConfigRequest);
    server.on("/relay", HTTP_GET, handle_GetRelayMode);

    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
        "/config",
        handle_PostConfigRequest);
    handler->setMethod(HTTP_POST);

    ESP32WebServer::server.begin();
    server.addHandler(handler);

    logger.log("Server started on port 80");
};

void ESP32WebServer::handle_GetConfigRequest(AsyncWebServerRequest *request)
{
    auto remote_ip = request->client()->remoteIP();
    logger.logf("New request from IP: %s", remote_ip);

    const char *arg = request->argName(0).c_str();
    Config *config = Config::getByName(arg);

    if (config == nullptr)
    {
        Config::load(arg);
        return;
    }

    char response[256] = "";
    serializeJson(config->data, response);
    logger.log(response);

    request->send(
        200,
        "application/json",
        response);
}

void ESP32WebServer::handle_PostConfigRequest(AsyncWebServerRequest *request, JsonVariant &json)
{
    logger.log("Config update requested - updating.");
    JsonObject obj = json.as<JsonObject>();

    const char *arg = request->argName(0).c_str();
    Config *config = Config::getByName(arg);

    config->data = obj;
    serializeJsonPretty(obj, Serial);

    int status = config->save();
    if (status != CONFIG_SAVED)
    {
        logger.logf("Error during config update: %d", status);
        request->send(500);
    }

    logger.log("File saved.");
    request->send(200);
}

void ESP32WebServer::handle_GetRelayMode(AsyncWebServerRequest *request)
{
    auto arg = request->getParam(0)->value();

    int pin = arg.toInt();
    int val = digitalRead(pin);

    logger.logf("Getting pin %d mode", pin);

    StaticJsonDocument<200> doc;
    JsonObject result = doc.to<JsonObject>();

    result["pin"] = pin;
    result["status"] = val;

    char response[256];
    serializeJson(doc, response);
    request->send(
        200, 
        "application/json", 
        response);
}
