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

        uint8_t id() {return _device_id;};
        void login();
        void registerDevice();
        void sync(char *buf);
        
        int postData(const JsonVariant &obj, const char *endpoint);
        void postReadings(const JsonVariant &obj);
        void postLog(const JsonVariant &obj);

    protected:
        Logger logger = Logger("SmartHomeDevice");
        char _host_url[60];
        uint8_t _device_id;

};

#endif
