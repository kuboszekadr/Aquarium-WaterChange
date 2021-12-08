#include "Device.h"

SmartHomeDevice *device;

void setupAPI()
{
  Config config = Config("api");
  config.load();

  device = new SmartHomeDevice(
      config.data["host"],
      config.data["port"].as<int>(),
      1);
}

void setupWiFi()
{
  WiFi.mode(WIFI_STA);

  Config config = Config("wifi");
  config.load();

  WiFiManager::connect(
      config.data["ssid"],
      config.data["pwd"]);

  ESP32WebServer::start();
}

void setupTime()
{
  if (device == nullptr)
  {
    logger.log("Can not sync time with the server.");
    return;
  }

  // send request to host server
  char timestamp[60];
  device->sync(timestamp);

  // deserialize response
  StaticJsonDocument<128> doc;
  deserializeJson(doc, timestamp);
  long epoch = doc["date"];

  // set device time
  ESP32Time _time = ESP32Time();
  _time.setTime(epoch);

  // String tm = _time.getDateTime();
  logger.log("Time set sucessfully");
}

void setupSPIFSS()
{
  if (!SPIFFS.begin())
  {
    logger.log("Failed to mount file system.");
  }
}