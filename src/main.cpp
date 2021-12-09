#include "Device.h"
#include "WaterChange.h"
#include "WaterLevel.h"
#include "Logger.h"

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
void streamToSerial(const char *module_name,
              const char *log_level,
              const char *msg,
              const char *timestamp);
Logger logger = Logger("main");

void setup()
{
  Serial.begin(115200);

  Device::setupSPIFSS();
  Device::setupWiFi();
  Device::setupAPI();

  Device::setupTime();
  Logger::addStream(streamToSerial);
  
  setupTasks();
  setupSensor();
  TaskScheduler::loadConfig();

  logger.log("Setup complete");
}

void loop()
{
  scheduler.loop();
  Sensors::loop();

  Events::notifyListeners();
  sendData();

  WiFiManager::manageConnection();
}

void setupTasks()
{
  logger.log("Setting tasks...");
  TaskScheduler::Task *time_sync = new TaskScheduler::Task("time_sync", Device::setupTime);
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
    Device::device->sendData(data);
    Sensors::readings.clear();
  }
}

void setupSensor()
{
  logger.log("Setting sensors...");

  Config sensor_config = Config("sensor");
  sensor_config.load();

  water_level_sensor.setSampling(
      sensor_config.data["sampling_amount"] ,
      sensor_config.data["sampling_interval"]);

  water_level_sensor.setTriggerValues(
      sensor_config.data["trigger_low"],
      sensor_config.data["trigger_high"]);
}

void streamToSerial(const char *module_name,
                    const char *log_level,
                    const char *msg,
                    const char *timestamp)
{
  char _msg[256];
  sprintf(_msg,
          "%s | %s | [%s] %s",
          module_name, log_level, timestamp, msg);
  Serial.println(_msg);
}