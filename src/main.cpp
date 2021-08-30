#include "Config.h"
#include "ESP32WebServer.h"
#include "WiFiManager.h"

#include <SPIFFS.h>
#include <WiFi.h>
#include <ESP32Time.h>

int status = WL_IDLE_STATUS;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system.");
  }

  Config::load("wifi");
  Config *wifi_config = Config::getByName("wifi");

  WiFiManager::connect(wifi_config->data["ssid"], wifi_config->data["pwd"]);
  ESP32WebServer::start();

}

void loop()
{
}

