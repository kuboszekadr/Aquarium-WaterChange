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

    doc["constant_level"] = false;

    serializeJson(doc, *response);
    request->send(response);
}

void Services::ServiceWaterManager::post(AsyncWebServerRequest *request, JsonVariant &json)
{

    JsonObject obj = json.as<JsonObject>();
    JsonDocument doc;

    if (obj.containsKey("constant_level"))
    {
        bool constant_level = obj["constant_level"];
        doc["status"] = "ok";
    }
    else
    {
        doc["status"] = "error";
        doc["message"] = "Missing 'constant_level' field in request body";
    }

    doc["status"] = "ok";

    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(doc, *response);
    request->send(response);
}
