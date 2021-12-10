#include "SmartHomeDevice.h"

SmartHomeDevice::SmartHomeDevice(const char *host, int port, uint8_t device_id)
{
    sprintf(_host_url, "http://%s:%d/api", host, port);
    _device_id = device_id;
}

void SmartHomeDevice::postReadings(const JsonVariant &obj)
{
    StaticJsonDocument<1000> doc;
    JsonObject _obj = doc.to<JsonObject>();

    _obj["device_id"] = _device_id;
    _obj["data"] = obj; 

    postData(_obj, "data_collector");
}

void SmartHomeDevice::postLog(const JsonVariant &obj)
{
    postData(obj, "logs");
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

void SmartHomeDevice::sync(char *buf)
{
    logger.log("Syncing time with the server...");

    char endpoint[60];
    sprintf(endpoint, "%s/%s", _host_url, "date");

    HTTPClient client;
    client.begin(endpoint);

    int response_code = client.GET();
    if (response_code != 200)
    {
        
        logger.log("Cannot sync device.");
        return;
    }

    client.getString().toCharArray(buf, 60);
    client.end();

    logger.log("Sync succesfull.");
}