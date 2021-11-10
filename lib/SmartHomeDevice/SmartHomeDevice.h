#ifndef SmartHomeDevice__h
#define SmartHomeDevice__h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <Logger.h>

class SmartHomeDevice
{
    public:
        SmartHomeDevice(const char *host, int port, uint8_t device_id);
        ~SmartHomeDevice();

        void login();
        void sendData(const JsonVariant &obj);
        void registerDevice();
        void sync(char *buf);

    protected:
        char _host_url[60];
        uint8_t _device_id;

};

#endif
