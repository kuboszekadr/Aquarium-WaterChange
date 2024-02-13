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
        Programs::water_manager.start();
        doc["status"] = "Water change started";
    }
    else if (action == "stop")
    {
        // Stop water change
        Programs::water_manager.terminate();
        doc["status"] = "Water change stopped";
    }
    else if (action == "deactivate")
    {
        // Deactivate water change
        Programs::water_manager.deactivate();
        Programs::water_manager.saveConfig();
        doc["status"] = "Water change deactivated";
    }
    else
    {
        request->send(
            500,
            "application/json",
            "{\"status\": \"error\", \"message\": \"Invalid action\"}"
            );
    }
    
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    serializeJson(doc, *response);
    request->send(response);
}
