#include "Device.h"
#include "WaterChange.h"
#include "WaterLevel.h"
#include "Logger.h"
#include "Notification.h"

#include <Arduino.h>
#include <Events.h>
#include <Sensors.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

int status = WL_IDLE_STATUS;

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
void streamToAPI(const char *module_name,
                 const char *log_level,
                 const char *msg,
                 const char *timestamp);

void GmailNotification(
    const char *title,
    const char *message);

Logger logger = Logger("main");

void setup()
{
  Serial.begin(115200);

  Device::setupSPIFSS();
  Device::setupWiFi();
  Device::setupAPI();
  Device::setupTime();

  Logger::addStream(streamToSerial);
  Logger::addStream(streamToAPI);

  Notification::addStream(GmailNotification);

  setupTasks();
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

  WiFiManager::manageConnection();
}

void setupTasks()
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
}

void sendData()
{
  if (Sensors::readings.size() > 0)
  {
    JsonVariant data = Sensors::readings.as<JsonVariant>();
    Device::device->postReadings(data);
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

void streamToAPI(const char *module_name,
                 const char *log_level,
                 const char *msg,
                 const char *timestamp)
{
  StaticJsonDocument<512> doc;
  JsonObject obj = doc.to<JsonObject>();

  obj["device_id"] = Device::device->id();
  obj["module_name"] = module_name;
  obj["log_level"] = log_level;
  obj["msg"] = msg;
  obj["log_timestamp"] = timestamp;

  Device::device->postLog(obj);
}

void GmailNotification(const char *title, const char *message)
{
  Device::device->postNotification(title, message);
}