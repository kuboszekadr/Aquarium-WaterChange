#include "Device.h"

SmartHomeDevice *Device::device;
Logger Device::logger = Logger("Device");

void Device::setupAPI()
{
    Config config = Config("api");
    config.load();

    String name = config.data["device_name"];
    device = new SmartHomeDevice(
        config.data["host"],
        config.data["port"].as<int>(),
        name);

    WiFi.setHostname(name.c_str());
}

void Device::setupWiFi()
{
    WiFi.mode(WIFI_STA);

    Config config = Config("wifi");
    config.load();

    WiFiManager::connect(
        config.data["ssid"],
        config.data["pwd"]);
}

void Device::setupTime()
{
    logger.log("Syncing time...");
    if (device == nullptr)
    {
        logger.log("Can not sync time with the server.");
        return;
    }

    // send request to host server
    JsonDocument doc;
    uint8_t timeout = 0;

    while (timeout < 10)
    {
        if (!device->sync(doc))
        {
            logger.log("Retrying to sync...");
        }
        else
        {
            break;
        }
        timeout += 1;
        delay(100);
    }

    long epoch = doc["date"];
    int timezone = doc["timezone"];

    char tz[10];
    if (timezone > 0) // trial and fail obtained...
    {
        sprintf(tz, "UTC-%i", timezone);
    }
    else
    {
        sprintf(tz, "UTC+%i", -timezone);
    }

    setTime(epoch, tz);
    logger.log("Time set sucessfully");
}

void Device::setupSPIFSS()
{
    if (!SPIFFS.begin())
    {
        logger.log("Failed to mount file system.");
    }
}

void Device::sendHeartbeat()
{
    JsonDocument doc;
    JsonObject obj = doc.to<JsonObject>();

    obj["device_name"] = device->device_name;
    (void)device->postData(obj, "api/v1.0/heartbeat");
}

void Device::setup()
{
    setupSPIFSS();
    setupWiFi();

    setupAPI();
    setupTime();
}