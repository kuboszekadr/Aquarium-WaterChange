#include "Device.h"

SmartHomeDevice *device;
ESP32Time _time = ESP32Time();

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

  // TODO: clean up
  char timestamp[60];
  device->sync(timestamp);

  char year[5];
  char month[3];
  char day[3];

  char hour[3];
  char minute[3];
  char second[3];

  memcpy(year, timestamp, 4);
  memcpy(month, &timestamp[4], 2);
  memcpy(day, &timestamp[6], 2);

  memcpy(hour, &timestamp[9], 2);
  memcpy(minute, &timestamp[11], 2);
  memcpy(second, &timestamp[13], 2);

  _time.setTime(
      atoi(second),
      atoi(minute),
      atoi(hour),
      atoi(day),
      atoi(month),
      atoi(year));

  String tm = _time.getDateTime();
  logger.log("Time set sucessfully");
}

void setupSPIFSS()
{
  if (!SPIFFS.begin())
  {
    logger.log("Failed to mount file system.");
  }
}