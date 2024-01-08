#ifndef Device_h
#define Device_h

#include "Config/Config.h"
#include "SmartHomeDevice/SmartHomeDevice.h"
#include "WiFiManager/WiFiManager.h"
#include "Logger/Logger.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESP32Time.h>

namespace Device
{
		extern SmartHomeDevice *device;
		extern Logger logger;

		void setupAPI();
		void setupWiFi();
		void setupTime();
		void setupSPIFSS();
		void sendHeartbeat();

		void setup();
}

#endif