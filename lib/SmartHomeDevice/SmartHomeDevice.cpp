#include "SmartHomeDevice.h"

SmartHomeDevice::SmartHomeDevice(const char *host, const char *port)
{
    memcpy(_host, host, 11);
    memcpy(_port, port, 11);
}

void SmartHomeDevice::sendData(JsonDocument &data)
{
    char url[60];
    sprintf(url, "http://%s:%s/api/data_collector", _host, _port);

    HTTPClient client;
    client.begin(url);
    client.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String payload;
    serializeJson(data, payload);
    int response_code = client.POST(payload);

    client.end();
    Serial.printf("Server response code: %d", response_code);
}