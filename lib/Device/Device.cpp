#include "Device.h"

SmartHomeDevice *Device::device;
Logger Device::logger = Logger("Device");

void Device::setupAPI()
{
  Config config = Config("api");
  config.load();

  device = new SmartHomeDevice(
      config.data["host"],
      config.data["port"].as<int>(),
      config.data["device_id"].as<int>());

  String test = config.data["host"];
}

void Device::setupWiFi()
{
  WiFi.mode(WIFI_STA);

  Config config = Config("wifi");
  config.load();

  WiFiManager::connect(
      config.data["ssid"],
      config.data["pwd"]);

  ESP32WebServer::start();
}

void Device::setupTime()
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
  // TODO: Add timezone

  // set device time
  ESP32Time _time = ESP32Time();
  _time.setTime(epoch);

  logger.log("Time set sucessfully");
}

void Device::setupSPIFSS()
{
  if (!SPIFFS.begin())
  {
    logger.log("Failed to mount file system.");
  }
}
