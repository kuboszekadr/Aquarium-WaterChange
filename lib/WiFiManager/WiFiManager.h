#ifndef WiFi_Manager__h
#define WiFi_Manager__h

#define CONNECTION_TIMEOUT 60000 // in miliseconds

#include <WiFi.h>

namespace WiFiManager
{
    bool connect(const char *ssid, const char *pwd);
    inline IPAddress get_ip() { return WiFi.localIP(); };

}

#endif