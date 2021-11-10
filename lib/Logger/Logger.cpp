#include "Logger.h"

Logger logger = Logger::getInstance();

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::log(const char *msg)
{
    logf("%s", msg);
}

void Logger::logf(const char *format, ...)
{
    char msg[128];

    ESP32Time t;
    struct tm timestamp = t.getTimeStruct();
    strftime(msg, 32, _date_format, &timestamp);

    va_list args;
    va_start(args, format);

    vsprintf(msg + strlen(msg), format, args);
    va_end(args);

    _stream(msg);
}

void Logger::setStream(stream s)
{
    _streams.push_back(s);
}

void Logger::setDateFormat(const char *format)
{
    memset(_date_format, 0, 16);
    memcpy(_date_format, format, 15);
}

void Logger::_stream(const char *msg)
{
    for (stream s : _streams)
    {
        s(msg);
    }
}