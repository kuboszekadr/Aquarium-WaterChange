#include "WiFiManager.h"

bool WiFiManager::connect(const char *ssid, const char *pwd)
{
    WiFi.begin(ssid, pwd);

    logger.log("Connecting to the target WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    logger.logf("Connected, device IP: %s",
                WiFiManager::getIP());

    bool result = WiFi.status() == WL_CONNECTED;
    return result;
}