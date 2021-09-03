#include "SmartHomeDevice.h"

SmartHomeDevice::SmartHomeDevice(const char *host, const char *port)
{
    memcpy(_host, host, 15);
    memcpy(_port, port, 11);
}

void SmartHomeDevice::sendData(JsonDocument &data)
{
    Serial.println("Sending data to API...");
    char url[60];
    sprintf(url, "http://%s:%s/api/data_collector", _host, _port);

    HTTPClient client;
    client.begin(url);
    client.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String payload;
    serializeJson(data, payload);
    int response_code = client.POST("data=" + payload); //TODO

    if (response_code != 200)
    {
        Serial.printf("Error during sending request. Server response code: %d\n", response_code);
        Serial.printf("Response content:\n%s\n", payload);
    }
    
    client.end();
    Serial.printf("Server response code: %d\n", response_code);
}