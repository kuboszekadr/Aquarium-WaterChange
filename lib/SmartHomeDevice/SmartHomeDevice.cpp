#include "SmartHomeDevice.h"

SmartHomeDevice::SmartHomeDevice(const char *host, int port, uint8_t device_id)
{
    sprintf(_host_url, "http://%s:%d/api", host, port);
    _device_id = device_id;
}

void SmartHomeDevice::sendData(const JsonVariant &obj)
{
    logger.log("Sending data to API...");
    char endpoint[60];
    sprintf(endpoint, "%s/%s", _host_url, "data_collector");

    HTTPClient client;
    client.begin(endpoint);
    client.addHeader("Content-Type", "application/json");

    DynamicJsonDocument doc(2222);
    doc["device_id"] = _device_id;
    doc["data"] = obj; 

    String payload;
    serializeJson(doc, payload); 
    
    int response_code = client.POST(payload);

    if (response_code != 200)
    {
        logger.logf("Error during sending request. Server response code: %d", response_code);
    }
    
    client.end();
    logger.logf("Server response code: %d", response_code);
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