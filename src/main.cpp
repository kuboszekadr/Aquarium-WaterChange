#include "Config.h"
#include "ESP32WebServer.h"
#include "SmartHomeDevice.h"
#include "WiFiManager.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESP32Time.h>

int status = WL_IDLE_STATUS;
SmartHomeDevice *device;

ESP32Time _time = ESP32Time();

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system.");
  }

  Config::load("wifi");
  Config *wifi_config = Config::getByName("wifi");

  WiFiManager::connect(wifi_config->data["ssid"], wifi_config->data["pwd"]);
  ESP32WebServer::start();

  Config::load("api");
  Config *api_config = Config::getByName("api");

  if (api_config == nullptr)
  {
    Serial.println("API config not available");
  } 
  else
  {
    Serial.println("API setup completed");

    device = new SmartHomeDevice(
      api_config->data["host"], 
      api_config->data["port"].as<int>());
  }

  _time.setTime(0, 0, 0, 6, 9, 2021);

  String tm = _time.getDateTime();
  Serial.println(tm);
}

void loop()
{
  StaticJsonDocument<256> doc;
  doc["device_id"] = 1;

  JsonArray data = doc.createNestedArray("data");
  JsonObject sensor_data = data.createNestedObject();

  sensor_data["sensor_id"] = -1;
  sensor_data["timestamp"] = _time.getTime("%F %T");

  JsonArray readings = sensor_data.createNestedArray("readings");
  JsonObject reading = readings.createNestedObject();
  reading["measure_id"] = -1;
  reading["value"] = -1;

  if (device != nullptr)
  {
    serializeJson(doc, Serial);
    device->sendData(doc);
  }
  delay(5000);
}
