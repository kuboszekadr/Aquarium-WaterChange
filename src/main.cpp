#include "Config.h"
#include "WiFiManager.h"

#include <SPIFFS.h>
#include <WiFi.h>

int status = WL_IDLE_STATUS;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system.");
  }

  Config::load();
  WiFiManager::connect(Config::data["ssid"], Config::data["pwd"]);
}

void loop()
{
}

