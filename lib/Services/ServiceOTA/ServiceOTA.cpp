#include "ServiceOTA.h"

void Services::ServiceOTA::create()
{
    server.on("/update", HTTP_POST, updateOnRequest, updateOnUpload);
}

void Services::ServiceOTA::updateOnRequest(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse *response = request->beginResponse(
        (Update.hasError()) ? 500 : 200,
        "text/plain",
        (Update.hasError()) ? "FAIL" : "OK");
    response->addHeader("Connection", "close");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);

    ESP.restart();
}

void Services::ServiceOTA::updateOnUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (!index)
    {
        int cmd = (filename == "filesystem") ? U_SPIFFS : U_FLASH;
        if (!Update.begin(UPDATE_SIZE_UNKNOWN, cmd))
        {
            // Start with max available size
            Update.printError(Serial);
            return request->send(400, "text/plain", "OTA could not begin");
        }
    }

    // Write chunked data to the free sketch space
    if (len)
    {
        if (Update.write(data, len) != len)
        {
            return request->send(400, "text/plain", "OTA could not begin");
        }
    }

    if (final)
    {
        // if the final flag is set then this is the last frame of data
        if (!Update.end(true))
        { // true to set the size to the current progress
            Update.printError(Serial);
            return request->send(400, "text/plain", "Could not end OTA");
        }
    }
    else
    {
        return;
    }
}