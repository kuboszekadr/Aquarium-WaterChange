#include "Config.h"
#include "ESP32WebServer.h"
#include "Events.h"
#include "Sensors.h"
#include "SmartHomeDevice.h"
#include "Task.h"
#include "TaskScheduler.h"
#include "TaskLoader.h"
#include "WiFiManager.h"
#include "WaterChange.h"
#include "WaterLevel.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <ESP32Time.h>

int status = WL_IDLE_STATUS;
SmartHomeDevice *device;

ESP32Time _time = ESP32Time();
TaskScheduler::Scheduler scheduler = TaskScheduler::Scheduler::getInstance();

#define WATER_LEVEL_LOW 20.0
#define WATER_LEVEL_HIGH 10.0

uint8_t water_level_sensor_id = 1;
Sensors::WaterLevel water_level_sensor(
    25,               // echo
    26,               // trig
    1,                // sensor_id
    "WaterLevelSump", // sensor_name
    (float)WATER_LEVEL_LOW,
    (float)WATER_LEVEL_HIGH,
    1000L,
    30);

Programs::WaterChange water_change = Programs::WaterChange(33, 32, 1);

void setupAPI();
void setupWiFI();
void setupTime();
void setupTasks();
void sampleTask();
void sendData();

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

  TaskScheduler::loadConfig();
}

void loop()
{
  scheduler.loop();
  Sensors::loop();
  Events::notifyListeners();

  sendData();
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
        api_config->data["port"].as<int>(),
        1);
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
  TaskScheduler::Task *time_sync = new TaskScheduler::Task("time_sync", setupTime);
  time_sync->schedule(400);

  TaskScheduler::Task *water_change_task = new TaskScheduler::Task("water_change", []()
                                                                   { water_change.start(); });
  water_change_task->schedule(900);
}

void sendData()
{
  if (Sensors::readings.size() > 0)
  {
    JsonVariant data = Sensors::readings.as<JsonVariant>();
    device->sendData(data);
    Sensors::readings.clear();
  }
}