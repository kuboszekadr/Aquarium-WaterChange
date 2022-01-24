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
  StaticJsonDocument<256> doc;
  device->sync(doc);

  long epoch = doc["date"];
  uint8_t timezone = doc["timezone"];

  // set device time
  ESP32Time _time = ESP32Time();
  _time.setTime(epoch);

  char tz[10];
  if (timezone < 0)
  {
    sprintf(tz, "UTC%i", timezone);
  }
  else
  {
    sprintf(tz, "UTC+%i", timezone);
  }

  setenv("TZ", tz, timezone);
  logger.log("Time set sucessfully");
}

void Device::setupSPIFSS()
{
  if (!SPIFFS.begin())
  {
    logger.log("Failed to mount file system.");
  }
}
