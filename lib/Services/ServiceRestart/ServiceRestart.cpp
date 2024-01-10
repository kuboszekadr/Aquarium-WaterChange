#include "ServiceRestart.h"

void Services::ServiceRestart::create()
{
    server.on("/restart", HTTP_GET, get);
}

void Services::ServiceRestart::get(AsyncWebServerRequest *request)
{
    request->send(
        200, 
        "text/plain",
        "restarting..."
        );
    delay(100);
    ESP.restart();
}

