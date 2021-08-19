#include "WiFiManager.h"

bool WiFi_Manager::connect(const char *ssid, const char *pwd)
{
    WiFi.begin(ssid, pwd);

    unsigned long timeout_start = millis();

    Serial.print("Connecting");

    while ((WiFi.status() != WL_CONNECTED) &&
           (millis() - timeout_start <= CONNECTION_TIMEOUT))
    {
        Serial.print('.');
        delay(500);
    }

    bool result = WiFi.status() == WL_CONNECTED;
    return result;
}