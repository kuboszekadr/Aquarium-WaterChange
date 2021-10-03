#ifndef Device_h
#define Device_h

#include "Config.h"
#include "SmartHomeDevice.h"
#include "ESP32WebServer.h"
#include "WiFiManager.h"

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESP32Time.h>

extern SmartHomeDevice *device;
extern ESP32Time _time;

void setupAPI();
void setupWiFi();
void setupTime();
void setupSPIFSS();

#endif