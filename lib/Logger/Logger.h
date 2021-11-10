#ifndef Logger__h
#define Logger__h

#include <ESP32Time.h>
#include <vector>

typedef void (*stream)(const char *mgs);
class Logger
{

public:
    static Logger &getInstance();

    void log(const char *msg);
    void logf(const char *format, ...);
    void setStream(stream s);
    void setDateFormat(const char *format);

private:
    Logger(){};
    std::vector<stream> _streams;

    char _date_format[16] = "%F %T | ";

    void _stream(const char *msg);
};

extern Logger logger;

#endif