#include "ServiceWaterManager.h"

void Services::ServiceWaterManager::create()
{
    server.on("/watermanager", HTTP_GET, get);

    AsyncCallbackJsonWebHandler *post_handler = new AsyncCallbackJsonWebHandler(
        "/watermanager",
        post);
    post_handler->setMethod(HTTP_POST);
    server.addHandler(post_handler);
}

void Services::ServiceWaterManager::get(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    JsonDocument doc;

    doc["keep_water_level"] = water_level_sensor.keep_water_level;

    serializeJson(doc, *response);
    request->send(response);
}

void Services::ServiceWaterManager::post(AsyncWebServerRequest *request, JsonVariant &json)
{

    JsonDocument doc;

    if (json.containsKey("keep_water_level"))
    {
        water_level_sensor.keep_water_level = json["keep_water_level"];
        water_level_sensor.saveConfig();
        constant_water_level_setup = water_level_sensor.keep_water_level;
        
        doc["status"] = "ok";
    }
    else
    {
        doc["status"] = "error";
        doc["message"] = "Missing 'keep_water_level' field in request body";
    }

    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(doc, *response);
    request->send(response);
}
