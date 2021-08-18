#include "Config.h"

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

    const char* ssid = Config::data["ssid"];
    const char* pwd = Config::data["pwd"];

    Serial.println(ssid);
    Serial.println(pwd);

    WiFi.begin(ssid, pwd);

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {   
        Serial.print(".");
        delay(500);
    }
    
    Serial.println(status);
    Serial.println("Setup done");
}

void loop()
{
}