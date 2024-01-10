#ifndef WiFi_Manager__h
#define WiFi_Manager__h

#define CONNECTION_TIMEOUT 60000 // in miliseconds

#include "../Logger/Logger.h"
#include <WiFi.h>

namespace WiFiManager
{
    extern Logger logger;

    wl_status_t connect(const char *ssid, const char *pwd);
    void manageConnection();
    
    inline IPAddress getIP() { return WiFi.localIP(); };
}

#endif