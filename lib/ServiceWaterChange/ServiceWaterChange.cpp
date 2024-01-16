#include "ServiceWaterChange.h"

void Services::ServiceWaterChange::create()
{
    AsyncCallbackJsonWebHandler *post_handler = new AsyncCallbackJsonWebHandler(
        "/waterchange",
        post);
    post_handler->setMethod(HTTP_POST);
    server.addHandler(post_handler);
}

void Services::ServiceWaterChange::post(AsyncWebServerRequest *request, JsonVariant &json)
{
    JsonObject obj = json.as<JsonObject>();
    String action = obj["action"].as<String>();
    JsonDocument doc;

    if (action == "start")
    {
        // Start water change
        Programs::water_change.start();
        doc["status"] = "Water change started";
    }
    else if (action == "stop")
    {
        // Stop water change
        Programs::water_change.stop();
        doc["status"] = "Water change stopped";
    }
    
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(doc, *response);
    request->send(response);
}
