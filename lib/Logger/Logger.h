#ifndef Logger__h
#define Logger__h

#include <cstring>
#include <vector>
#include <cstdarg>
#include <cstdio>
#include <stdio.h>

#include "../Timestamp/Timestamp.h"

typedef void (*stream)(const char *module_name,
                       const char *log_level,
                       const char *msg,
                       const char *timestamp);

extern std::vector<stream> streams;

const char date_format[16] = "%F %T";

class Logger
{
public:
    Logger(const char *module_name);

    void log(const char *msg);
    void logf(const char *msg_format, ...);

    static void addStream(stream s);

private:
    char _log_level[32] = "INFO"; // placeholder
    char _module_name[32] = "";

    void _stream(const char *module_name,
                 const char *log_level,
                 const char *msg,
                 const char *timestamp);
};

#endif