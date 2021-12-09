#include "WiFiManager.h"

Logger WiFiManager::logger = Logger("wifimanager");

wl_status_t WiFiManager::connect(const char *ssid, const char *pwd)
{
    WiFi.begin(ssid, pwd);
    return waitForConnection();
}

void WiFiManager::manageConnection()
{
    int status = WiFi.status();
    if (status != WL_CONNECTED)
    {
        logger.log("Reconecting...");
        WiFi.begin();
        waitForConnection();
    }
}

wl_status_t WiFiManager::waitForConnection()
{
    long timeout = millis();

    while (
        WiFi.status() != WL_CONNECTED ||
        isTimeouted(timeout, CONNECTION_TIMEOUT))
    {
        delay(100);
    }

    return WiFi.status();
}

bool WiFiManager::isTimeouted(long connection_start, long timeout)
{
    bool result = (millis() - connection_start > timeout);
    return result;
}