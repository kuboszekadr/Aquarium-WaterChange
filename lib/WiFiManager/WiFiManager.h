#ifndef WiFi_Manager__h
#define WiFi_Manager__h

#include <WiFi.h>

#define CONNECTION_TIMEOUT 60000 // in miliseconds

namespace WiFi_Manager
{
    bool connect(const char *ssid, const char *pwd);
}

#endif