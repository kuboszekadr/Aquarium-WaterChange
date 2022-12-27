#include "Device/Device.h"
#include "Sensors/Sensors.h"
#include "ESP32WebServer/ESP32WebServer.h"

#include "Logger/Logger.h"
#include "Logger/Loggers/API.hpp"
#include "Logger/Loggers/Serial.hpp"

#include "Notification/Notification.h"
#include "Events/Events.h"

#include "WaterChange.h"
#include "WaterLevel.h"
#include "Pins.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

int status = WL_IDLE_STATUS;

Sensors::WaterLevel water_level_sensor(
    HCSR04_ECHO,
    HCSR04_TRIG,
    1,
    "WaterLevelSump");

Programs::WaterChange water_change = Programs::WaterChange(
    WATER_FLOW_OUT,
    WATER_FLOW_IN,
    1);

void initTasks();
void setupSensor();
void sendData();

void GmailNotification(
    const char *title,
    const char *message);

Logger logger = Logger("main");

void setup()
{
  Serial.begin(115200);

  Device::setup();

  Logger::addStream(Loggers::logToSerial);
  Logger::addStream(Loggers::logToAPI);

  Notification::addStream(GmailNotification);

  initTasks();
  setupSensor();

  logger.log("Setup complete");
  Notification::push("WaterChange-init", "Device started");
}

void loop()
{
  Cron.delay(); 
  Sensors::loop();

  Events::notifyListeners();
  sendData();
}

void initTasks()
{
  logger.log("Setting tasks...");
  Cron.create(
      "0 0 4 * * *",
      Device::setupTime,
      false);

  Cron.create(
      "0 0 9 * * *",
      []()
      { water_change.start(); },
      false);

  Cron.create(
      "0 */2 * * * *",
      WiFiManager::manageConnection,
      false);          
}

void sendData()
{
  if (Sensors::readings.size() > 0)
  {
    JsonVariant data = Sensors::readings.as<JsonVariant>();
    Device::device->postReadings(data);
    ESP32WebServer::last_reading = data;
    Sensors::readings.clear();
  }
}

void setupSensor()
{
  logger.log("Setting sensors...");

  Config sensor_config = Config("sensor");
  sensor_config.load();

  water_level_sensor.setSampling(
      sensor_config.data["sampling_amount"],
      sensor_config.data["sampling_interval"]);

  water_level_sensor.setTriggerValues(
      sensor_config.data["trigger_low"],
      sensor_config.data["trigger_high"]);
}

void GmailNotification(const char *title, const char *message)
{
  Device::device->postNotification(title, message);
}