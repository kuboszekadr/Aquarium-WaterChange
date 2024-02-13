#include "Device/Device.h"
#include "Sensors/Sensors.h"

//Loggers
#include "Logger/Logger.h"
#include "Logger/Loggers/API.hpp"
#include "Logger/Loggers/Serial.hpp"

//Services
#include "Services/Services.h"
#include "Services/ServiceSystemTime/ServiceSystemTime.h"
#include "Services/ServiceConfig/ServiceConfig.h"
#include "Services/ServiceOTA/ServiceOTA.h"
#include "Services/ServiceRestart/ServiceRestart.h"



//Custom
#include "WaterManager.h"
#include "WaterLevel.h"
#include "Pins.h"

//Utils
#include "Notification/Notification.h"
#include "Events/Events.h"
#include "ESP32Time.h"

#include <Arduino.h>
#include <SPIFFS.h>
#include <CronAlarms.h>

#include <esp_task_wdt.h>

#define WDT_TIMEOUT 20
#define DEVELOP

#ifndef DEVELOP
    #define DEVICE_NAME "WaterManager"
#else
    #define DEVICE_NAME "WaterManager-Develop"
#endif

int status = WL_IDLE_STATUS;
bool constant_water_level_setup = false; // walkaround for the water level sensor

Sensors::WaterLevel water_level_sensor(
    HCSR04_ECHO,
    HCSR04_TRIG,
    "WaterLevelSump"
    );

#include "ServiceWaterManager.h"
#include "ServiceWaterChange.h"

Programs::WaterManager Programs::water_manager = Programs::WaterManager(
    WATER_FLOW_OUT,
    WATER_FLOW_IN
    );

void initTasks();
void setupSensor();
void sendData();
void startWaterChange();

void GmailNotification(
    const char *title,
    const char *message);

const char VERSION[8] = "v1.1.0";
Logger logger = Logger("main");

Services::ServiceSystemTime Services::time = Services::ServiceSystemTime();
Services::ServiceOTA Services::ota = Services::ServiceOTA();

Services::ServiceWaterManager Services::water_manager_service = Services::ServiceWaterManager();
Services::ServiceWaterChange Services::water_manager = Services::ServiceWaterChange();

Services::ServiceConfig Services::config = Services::ServiceConfig();
Services::ServiceRestart Services::restart = Services::ServiceRestart();

void setup()
{
    Serial.begin(115200);
    Logger::addStream(Loggers::logToSerial);
    Programs::water_manager.configure(WATER_FLOW_OUT, WATER_FLOW_IN);

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

    if (Programs::water_manager.isRunning())
    {
        water_level_sensor.keep_water_level = false;
    }
    else
    {
        water_level_sensor.keep_water_level = constant_water_level_setup;
    }

    esp_task_wdt_reset();
}

/**
 * @brief Initializes the tasks for the aquarium water change system.
 * 
 * Sets up the scheduled tasks using the Cron library, ,
 * creates tasks for setting up the device time, sending heartbeat,
 * starting water change, and managing WiFi connection.
 */
void initTasks()
{
    
    Config tasks = Config("tasks");
    tasks.load();

    logger.log("Setting tasks...");
    auto time_sync_config = tasks.data["timeSync"].as<const char*>();
    Cron.create(
        const_cast<char*>(time_sync_config),
        Device::setupTime,
        false);

    Cron.create(
        "*/30 * * * * *",
        Device::sendHeartbeat,
        false);

    auto water_change_config = tasks.data["startWaterChange"].as<const char*>();
    Cron.create(
        const_cast<char*>(water_change_config),
        startWaterChange,
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

/**
 * @brief Starts the water change process.
 * 
 * This function disables the constant water level monitoring, starts the water change program,
 * and restores the original state of the water level monitoring after the water change is complete.
 */
void startWaterChange()
{
    Programs::water_manager.start();
}

/**
 * @brief Sets up the sensor configuration and trigger values.
 * 
 * This function initializes the sensor configuration by loading the settings from a configuration file.
 */
void setupSensor()
{
    water_level_sensor.loadConfig();
    constant_water_level_setup = water_level_sensor.keep_water_level;
    
    Programs::water_manager.loadConfig();
}

/**
 * Sends a Gmail notification with the specified title and message.
 *
 * @param title The title of the notification.
 * @param message The message content of the notification.
 */
void GmailNotification(const char *title, const char *message)
{
    Device::device->postNotification(title, message);
}