#include "Device.h"

SmartHomeDevice *device;
ESP32Time _time = ESP32Time();

void setupAPI()
{
  Config::load("api");
  Config *api_config = Config::getByName("api");

  if (api_config == nullptr)
  {
    Serial.println("Unable to load API config.");
  }
  else
  {
    Serial.println("API setup completed.");

    device = new SmartHomeDevice(
        api_config->data["host"],
        api_config->data["port"].as<int>(),
        1);
  }
}

void setupWiFi()
{
  WiFi.mode(WIFI_STA);

  Config::load("wifi");
  Config *wifi_config = Config::getByName("wifi");

  WiFiManager::connect(wifi_config->data["ssid"], wifi_config->data["pwd"]);
  ESP32WebServer::start();
}

void setupTime()
{
  if (device == nullptr)
  {
    Serial.println("Can not sync time with the server.");
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
  Serial.println("Time set sucessfully");
}

void setupSPIFSS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system.");
  }
}