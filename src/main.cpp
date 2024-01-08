#include "Device/Device.h"
#include "Sensors/Sensors.h"
#include "ESP32WebServer/ESP32WebServer.h"

#include "Logger/Logger.h"
#include "Logger/Loggers/API.hpp"
#include "Logger/Loggers/Serial.hpp"

#include "Services/Services.h"
#include "Services/ServiceSystemTime/ServiceSystemTime.h"
#include "Services/ServiceConfig/ServiceConfig.h"
#include "Services/ServiceOTA/ServiceOTA.h"
#include "Services/ServiceRestart/ServiceRestart.h"

#include "ServiceWaterManager.h"
#include "ServiceWaterChange.h"

#include "Notification/Notification.h"
#include "Events/Events.h"

#include "WaterManager.h"
#include "WaterLevel.h"
#include "Pins.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

#include <esp_task_wdt.h>

#define WDT_TIMEOUT 20

int status = WL_IDLE_STATUS;

Sensors::WaterLevel water_level_sensor(
    HCSR04_ECHO,
    HCSR04_TRIG,
    1,
    "WaterLevelSump");

Programs::WaterManager Programs::water_change = Programs::WaterManager(
    WATER_FLOW_OUT,
    WATER_FLOW_IN,
    1);

void initTasks();
void setupSensor();
void sendData();

void GmailNotification(
    const char *title,
    const char *message);

const char VERSION[8] = "v1.0.0";
Logger logger = Logger("main");

Services::ServiceSystemTime service_time = Services::ServiceSystemTime();
Services::ServiceOTA service_ota = Services::ServiceOTA();

Services::ServiceWaterManager service_water_manager = Services::ServiceWaterManager();
Services::ServiceWaterChange service_water_change = Services::ServiceWaterChange();

Services::ServiceConfig config_service = Services::ServiceConfig();
Services::ServiceRestart service_restart = Services::ServiceRestart();

void setup()
{
    Serial.begin(115200);

    Programs::water_change.configure(WATER_FLOW_OUT, WATER_FLOW_IN);

    esp_task_wdt_init(WDT_TIMEOUT, true);
    esp_task_wdt_add(NULL);

    Device::setup();

    Logger::addStream(Loggers::logToSerial);
    Logger::addStream(Loggers::logToAPI);

    Notification::addStream(GmailNotification);

    Services::init();
    Services::server.begin();

    initTasks();
    setupSensor();

    char msg[64];
    snprintf(msg, 63, "Device started \nFirmware version: %s", VERSION);
    Notification::push("WaterManager-init", msg);
}

void loop()
{
    Cron.delay();
    Sensors::loop();

    Events::notifyListeners();
    sendData();

    esp_task_wdt_reset();
}

void initTasks()
{
    logger.log("Setting tasks...");
    Cron.create(
        "0 0 4 * * *",
        Device::setupTime,
        false);

    Cron.create(
        "*/30 * * * * *",
        Device::sendHeartbeat,
        false);

    Cron.create(
        "0 0 9 * * *",
        []()
        { Programs::water_change.start(); },
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
        Device::device->postReadings(data, "WaterLevel");
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