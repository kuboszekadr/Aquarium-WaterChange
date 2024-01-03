#include "ServiceWaterManager.h"

void Services::ServiceWaterManager::create()
{
    server.on("/watermanager", HTTP_GET, get);
    server.on("/watermanager", HTTP_POST, post);
}

void Services::ServiceWaterManager::get(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);

    doc["constant_level"] = true;

    serializeJson(doc, *response);
    request->send(response);
}

void Services::ServiceWaterManager::post(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);

    if (request->hasParam("constant_level"))
    {
        // water_change.constant_level = request->getParam("constant_level")->value().equals("true");
        // water_change.saveConfig();
    }

    doc["constant_level"] = false;

    serializeJson(doc, *response);
    request->send(response);
}