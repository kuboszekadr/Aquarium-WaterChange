#include "WiFiManager.h"

bool WiFiManager::connect(const char *ssid, const char *pwd)
{
    WiFi.begin(ssid, pwd);

    Serial.print(F("Connecting"));
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(500);
    }

    Serial.println("");
    Serial.print(F("Connection sucessful, device IP: "));
    Serial.println(WiFiManager::get_ip());

    bool result = WiFi.status() == WL_CONNECTED;
    return result;
}