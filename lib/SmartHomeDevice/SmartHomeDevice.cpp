#include "SmartHomeDevice.h"

SmartHomeDevice::SmartHomeDevice(const char *host, int port, String device_name)
{
    sprintf(host_url, "http://%s:%d", host, port);
    device_name.toCharArray(this->device_name, 31);
}

void SmartHomeDevice::postNotification(const char *title, const char *message)
{
    char url[60];
    sprintf(url, "%s/%s", host_url, "notifier");

    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();

    obj["title"] = title;
    obj["message"] = message;

    postData(obj, "notifier");
}

void SmartHomeDevice::postReadings(const JsonVariant &obj, const char *sensor_name)
{
    JsonDocument doc;
    JsonObject _obj = doc.to<JsonObject>();

    _obj["device_name"] = device_name;
    _obj["sensor_name"] = sensor_name;
    _obj["readings"] = obj;

    postData(_obj, "api/data_collector");
}

void SmartHomeDevice::postLog(const JsonVariant &obj)
{
    postData(obj, "api/v2.0/logs");
}

String SmartHomeDevice::getData(const JsonVariant &obj, const char *endpoint, const char *version)
{
    String payload;
    serializeJson(obj, payload);

    char url[60];
    sprintf(url, "%s/api/%s/%s", host_url, version, endpoint);

    HTTPClient client;
    client.begin(url);
    client.addHeader("Content-Type", "application/json");
    client.setConnectTimeout(1000);

    int response_code = client.sendRequest("GET", payload);
    if (response_code != 200)
    {
        logger.logf("Cannot get value from endpoint %s. Response code: %d", url, response_code);
        return String("0");
    }

    String result = client.getString();
    client.end();
    return result;
}

int SmartHomeDevice::postData(const JsonVariant &obj, const char *endpoint)
{
    String payload;
    serializeJson(obj, payload);
    Serial.println(payload);

    char url[60];
    sprintf(url, "%s/%s", host_url, endpoint);

    HTTPClient client;
    client.begin(url);
    client.setConnectTimeout(1000);
    client.addHeader("Content-Type", "application/json");

    int response_code = client.POST(payload);
    client.end();

    return response_code;
}

bool SmartHomeDevice::sync(JsonDocument &doc)
{
    logger.log("Syncing time with the server...");
    char endpoint[60];
    sprintf(endpoint, "%s/%s", host_url, "api/v1.0/date");

    HTTPClient client;
    client.begin(endpoint);
    client.setConnectTimeout(1000);

    int response_code = client.GET();
    if (response_code != 200)
    {

        logger.log("Cannot sync device.");
        return false;
    }

    deserializeJson(doc, client.getString());
    client.end();

    logger.log("Sync succesfull.");
    return true;
}
