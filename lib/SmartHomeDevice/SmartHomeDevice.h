#ifndef SmartHomeDevice__h
#define SmartHomeDevice__h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

class SmartHomeDevice
{
    public:
        SmartHomeDevice(const char *host, const char *port);
        ~SmartHomeDevice();

        void login();
        void sendData(JsonDocument &data);
        void registerDevice();
        void sync();

        uint8_t id;
    protected:
        char _host[16];
        char _port[5];
};

#endif