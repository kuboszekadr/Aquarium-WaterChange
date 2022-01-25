#include "SmartHomeDevice.h"

SmartHomeDevice::SmartHomeDevice(const char *host, int port, uint8_t device_id)
{
    sprintf(_host_url, "http://%s:%d", host, port);
    _device_id = device_id;
}

void SmartHomeDevice::postNotification(const char *title, const char *message)
{
    char url[60];
    sprintf(url, "%s/%s", _host_url, "notifier");
    
    StaticJsonDocument<512> doc;
    JsonObject obj = doc.to<JsonObject>();

    obj["title"] = title;
    obj["message"] = message;

    // postData(obj, "notifier"); //TODO: restore later-on
}

void SmartHomeDevice::postReadings(const JsonVariant &obj)
{
    StaticJsonDocument<1000> doc;
    JsonObject _obj = doc.to<JsonObject>();

    _obj["device_id"] = _device_id;
    _obj["data"] = obj; 

    postData(_obj, "api/data_collector");
}

void SmartHomeDevice::postLog(const JsonVariant &obj)
{
    postData(obj, "api/logs");
}

int SmartHomeDevice::postData(const JsonVariant &obj, const char *endpoint)
{
    String payload;
    serializeJson(obj, payload); 

    char url[60];
    sprintf(url, "%s/%s", _host_url, endpoint);

    HTTPClient client;
    client.begin(url);
    client.addHeader("Content-Type", "application/json");
    
    int response_code = client.POST(payload);    
    client.end();

    return response_code;
}

void SmartHomeDevice::sync(JsonDocument &doc)
{
    logger.log("Syncing time with the server...");

    char endpoint[60];
    sprintf(endpoint, "%s/%s", _host_url, "api/date");

    HTTPClient client;
    client.begin(endpoint);

    int response_code = client.GET();
    if (response_code != 200)
    {
        
        logger.log("Cannot sync device.");
        return;
    }

    deserializeJson(doc, client.getString());
    client.end();

    logger.log("Sync succesfull.");
}