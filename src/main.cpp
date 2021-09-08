#include "Config.h"
#include "ESP32WebServer.h"
#include "Events.h"
#include "SmartHomeDevice.h"
#include "WiFiManager.h"
#include "Task.h"
#include "TaskScheduler.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESP32Time.h>

int status = WL_IDLE_STATUS;
SmartHomeDevice *device;

ESP32Time _time = ESP32Time();
TaskScheduler::Scheduler scheduler = TaskScheduler::Scheduler::getInstance();

void setupAPI();
void setupWiFI();
void setupTime();
void setupTasks();
void sampleTask();

void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system.");
  }

  setupWiFI();
  setupAPI();
  setupTime();
  setupTasks();
}

void loop()
{
  // StaticJsonDocument<256> doc;
  // doc["device_id"] = 1;

  // JsonArray data = doc.createNestedArray("data");
  // JsonObject sensor_data = data.createNestedObject();

  // sensor_data["sensor_id"] = -1;
  // sensor_data["timestamp"] = _time.getTime("%F %T");

  // JsonArray readings = sensor_data.createNestedArray("readings");
  // JsonObject reading = readings.createNestedObject();
  // reading["measure_id"] = -1;
  // reading["value"] = -1;

  // if (device != nullptr)
  // {
  //   serializeJson(doc, Serial);
  //   device->sendData(doc);
  // }

  scheduler.loop();

  delay(5000);
}

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
        api_config->data["port"].as<int>());
  }
}

void setupWiFI()
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

void setupTasks()
{
  TaskScheduler::Task *task = new TaskScheduler::Task("TimeSync", setupTime);
  task->schedule(400);
}
