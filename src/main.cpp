#include "Sensors.h"
#include "Device.h"
#include "Task.h"
#include "TaskScheduler.h"
#include "TaskLoader.h"
#include "WaterChange.h"
#include "WaterLevel.h"

#include <Arduino.h>
#include <Events.h>
#include <SPIFFS.h>

int status = WL_IDLE_STATUS;

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

void setupTasks();
void sendData();

void setup()
{
  Serial.begin(115200);

  setupSPIFSS();
  setupWiFi();
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