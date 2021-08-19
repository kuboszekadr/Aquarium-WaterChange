#include "WiFiManager.h"

bool WiFiManager::connect(const char *ssid, const char *pwd)
{
    WiFi.begin(ssid, pwd);

    unsigned long timeout_start = millis();

    Serial.print("Connecting");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(500);
    }

    bool result = WiFi.status() == WL_CONNECTED;
    return result;
}