#ifndef LoggerSerial__h
#define LoggerSerial__h

#ifdef UNIT_TEST
    #include <iostream>
    #include <string>
    #include <sstream>
#else
    #include <Arduino.h>
#endif

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
                
        #ifdef UNIT_TEST                
            std::cout << _msg << std::endl;
        #else
            Serial.println(_msg);
        #endif
    }
}

#endif