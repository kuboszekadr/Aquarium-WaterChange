#ifndef WiFi_Manager__h
#define WiFi_Manager__h

#define CONNECTION_TIMEOUT 60000 // in miliseconds

#include <WiFi.h>
#include <Logger.h>

namespace WiFiManager
{
    bool connect(const char *ssid, const char *pwd);
    inline IPAddress getIP() { return WiFi.localIP(); };
}

#endif