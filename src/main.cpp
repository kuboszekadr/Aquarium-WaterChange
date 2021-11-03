#include "Device.h"
#include "WaterChange.h"
#include "WaterLevel.h"

#include <Arduino.h>
#include <Events.h>
#include <Sensors.h>
#include <Task.h>
#include <TaskLoader.h>
#include <TaskScheduler.h>
#include <SPIFFS.h>

int status = WL_IDLE_STATUS;

TaskScheduler::Scheduler scheduler = TaskScheduler::Scheduler::getInstance();

Sensors::WaterLevel water_level_sensor(
    25,              // echo
    26,              // trig
    1,               // sensor_id
    "WaterLevelSump" // sensor_name
);

Programs::WaterChange water_change = Programs::WaterChange(33, 32, 1);

void setupTasks();
void setupSensor();
void sendData();

void setup()
{
  Serial.begin(115200);

  setupSPIFSS();
  setupWiFi();
  setupAPI();

  setupTime();
  setupTasks();
  setupSensor();

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

void setupSensor()
{
  Config::load("sensor");
  Config *sensor_config = Config::getByName("sensor");

  water_level_sensor.setSampling(
      sensor_config->data["sampling_amount"],
      sensor_config->data["sampling_interval"]);

  water_level_sensor.setTriggerValues(
      sensor_config->data["trigger_low"],
      sensor_config->data["trigger_high"]);
}