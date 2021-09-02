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

  device = new SmartHomeDevice("192.168.0.107", "8080");

}

void loop()
{
  StaticJsonDocument<256> doc;
  doc["device_id"] = 1;

  JsonObject data = doc.createNestedObject("data");
  data["sensor_id"] = -1;
  data["timestamp"] = "20000000 000000";

  StaticJsonDocument<128> reading;
  reading["measure_id"] = -1;
  reading["value"] = -1.0;

  JsonArray readings = data.createNestedArray("readings");
  readings.add(reading);

  serializeJson(doc, Serial);

  delay(5000);
}

