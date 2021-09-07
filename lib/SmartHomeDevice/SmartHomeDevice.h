#ifndef SmartHomeDevice__h
#define SmartHomeDevice__h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

class SmartHomeDevice
{
    public:
        SmartHomeDevice(const char *host, int port);
        ~SmartHomeDevice();

        void login();
        void sendData(JsonDocument &data);
        void registerDevice();
        void sync(char *buf);

        uint8_t id;
    protected:
        char _host_url[60];

};

#endif
