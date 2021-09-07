#include "SmartHomeDevice.h"

SmartHomeDevice::SmartHomeDevice(const char *host, int port)
{
    sprintf(_host_url, "http://%s:%d/api", host, port);
}

void SmartHomeDevice::sendData(JsonDocument &data)
{
    Serial.println("Sending data to API...");
    char endpoint[60];
    sprintf(endpoint, "%s/%s", _host_url, "data_collector");
    Serial.println(endpoint);

    HTTPClient client;
    client.begin(endpoint);
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

void SmartHomeDevice::sync(char *buf)
{
    Serial.println("Syncing time with the server...");
    char endpoint[60];
    sprintf(endpoint, "%s/%s", _host_url, "date");

    HTTPClient client;
    client.begin(endpoint);

    int response_code = client.GET();

    if (response_code != 200)
    {
        Serial.println("Cannot sync device.");
        return;
    }

    client.getString().toCharArray(buf, 60);
    client.end();

    Serial.println("Sync succesfull.");
}