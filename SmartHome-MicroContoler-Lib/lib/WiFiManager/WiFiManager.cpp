#include "WiFiManager.h"

Logger WiFiManager::logger = Logger("wifimanager");

wl_status_t WiFiManager::connect(const char *ssid, const char *pwd)
{
    WiFi.begin(ssid, pwd);
    WiFiManager::manageConnection();
    return WiFi.status();
}

void WiFiManager::manageConnection()
{
    wl_status_t status = WiFi.status();
    uint8_t timeout = 0;
    while ((status != WL_CONNECTED) & (timeout < 10))
    {
        Serial.println("Connecting to WiFi...");
        status = WiFi.status();

        if (status == WL_CONNECTED)
        {
            Serial.println("Connected to target WiFi.");
        }

        timeout++; 
        delay(500);
    }
}