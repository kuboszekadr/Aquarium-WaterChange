#include "Device.h"
#include "Sensors.h"

//Loggers
#include "Logger.h"
#include "Loggers/API.hpp"
#include "Loggers/Serial.hpp"

//Services
#include "Services.h"
#include "ServiceSystemTime/ServiceSystemTime.h"
#include "ServiceConfig/ServiceConfig.h"
#include "ServiceOTA/ServiceOTA.h"
#include "ServiceRestart/ServiceRestart.h"
#include "ServiceWaterManager.h"
#include "ServiceWaterChange.h"

//Custom
#include "WaterManager.h"
#include "WaterLevel.h"
#include "Pins.h"

//Utils
#include "Notification.h"
#include "Events.h"
#include "ESP32Time.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

#include <esp_task_wdt.h>

#define WDT_TIMEOUT 20
#ifdef DEV
    #define DEVICE_NAME "WaterManager-Dev"
#else
    #define DEVICE_NAME "WaterManager"
#endif

int status = WL_IDLE_STATUS;

Sensors::WaterLevel water_level_sensor(
    HCSR04_ECHO,
    HCSR04_TRIG,
    "WaterLevelSump"
    );

Programs::WaterManager Programs::water_change = Programs::WaterManager(
    WATER_FLOW_OUT,
    WATER_FLOW_IN
    );

void initTasks();
void setupSensor();
void sendData();

void GmailNotification(
    const char *title,
    const char *message);

const char VERSION[8] = "v1.0.0";
Logger logger = Logger("main");

Services::ServiceSystemTime Services::time = Services::ServiceSystemTime();
Services::ServiceOTA Services::ota = Services::ServiceOTA();

Services::ServiceWaterManager Services::water_manager = Services::ServiceWaterManager();
Services::ServiceWaterChange Services::water_change = Services::ServiceWaterChange();

Services::ServiceConfig Services::config = Services::ServiceConfig();
Services::ServiceRestart Services::restart = Services::ServiceRestart();

void setup()
{
    Serial.begin(115200);

    Logger::addStream(Loggers::logToSerial);
    Programs::water_change.configure(WATER_FLOW_OUT, WATER_FLOW_IN);

    esp_task_wdt_init(WDT_TIMEOUT, true);
    esp_task_wdt_add(NULL);

    Device::setup();

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
        Device::device->postReadings(Sensors::readings, DEVICE_NAME);
        Sensors::readings.clear();
    }
}

void setupSensor()
{
    logger.log("Setting sensors...");

    Config sensor_config = Config("sensor");
    sensor_config.load();

    // String test;
    // serializeJson(sensor_config.data, test);
    // Serial.println(test);

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