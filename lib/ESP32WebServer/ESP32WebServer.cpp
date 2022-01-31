#include "ESP32WebServer.h"

AsyncWebServer ESP32WebServer::server(80);
Logger ESP32WebServer::logger = Logger("Webserver");
StaticJsonDocument<256> ESP32WebServer::last_reading;

void ESP32WebServer::start()
{
    server.on("/", [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/www/index.html", "text/html", false); });
    server.serveStatic("/", SPIFFS, "/www");

    server.on("/config", HTTP_GET, handle_GetConfigRequest);
    server.on("/relay", HTTP_GET, handle_GetRelayMode);
    server.on("/time", HTTP_GET, handle_GetSystemTime);
    server.on("/reading", HTTP_GET, handle_GetReadings);

    AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler(
        "/config",
        handle_PostConfigRequest);
    handler->setMethod(HTTP_POST);

    ESP32WebServer::server.begin();
    server.addHandler(handler);
};

void ESP32WebServer::handle_GetConfigRequest(AsyncWebServerRequest *request)
{
    auto remote_ip = request->client()->remoteIP();
    logger.logf("New request from IP: %s", remote_ip);

    const char *arg = request->argName(0).c_str();
    Config config = Config(arg);
    config.load();

    char response[256] = "";
    serializeJson(config.data, response);
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
    Config config = Config(arg);
    config.load();

    config.data = obj;
    serializeJsonPretty(obj, Serial);

    int status = config.save();
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

    StaticJsonDocument<200> doc;
    JsonArray result = doc.to<JsonArray>();

    for (const auto &relay : Relay::relays)
    {
        JsonObject pin = result.createNestedObject();
        pin["pin"] = relay.first;
        pin["status"] = relay.second->getState();
    }

    char response[256];
    serializeJson(doc, response);

    request->send(
        200,
        "application/json",
        response);
}

void ESP32WebServer::handle_GetSystemTime(AsyncWebServerRequest *request)
{
    ESP32Time t;
    String system_time = t.getTime("%F %X");

    StaticJsonDocument<80> doc;
    JsonObject result = doc.to<JsonObject>();
    result["time"] = system_time;

    char response[80];
    serializeJson(doc, response);

    request->send(
        200,
        "application/json",
        response);
}

void ESP32WebServer::handle_GetReadings(AsyncWebServerRequest *request)
{
    String response;
    serializeJson(last_reading, response);

    request->send(
        200,
        "application/json",
        response);
}