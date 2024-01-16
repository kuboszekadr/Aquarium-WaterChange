#ifndef LoggerSerial__h
#define LoggerSerial__h

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <iostream>
    #include <string>
    #include <sstream>
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
                
        #ifdef ARDUINO                
            Serial.println(_msg);
        #else
            std::cout << _msg << std::endl;
        #endif
    }
}

#endif