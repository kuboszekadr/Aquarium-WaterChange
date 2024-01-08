#ifndef LoggerSerial__h
#define LoggerSerial__h

#include <Arduino.h>

namespace Loggers
{
    void logToSerial(const char *module_name,
                const char *log_level,
                const char *msg,
                const char *timestamp)
    {
        char _msg[256];
        sprintf(_msg,
                "%s | %s | [%s] %s",
                module_name, log_level, timestamp, msg);
        Serial.println(_msg);
    }
}

#endif