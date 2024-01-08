#include "Logger.h"

std::vector<stream> streams = std::vector<stream>();

Logger::Logger(const char *module_name)
{
    memset(_module_name, 0, 32);
    memcpy(_module_name, module_name, 31);
}

void Logger::log(const char *msg)
{
    logf("%s", msg);
}

void Logger::logf(const char *msg_format, ...)
{
    // get current system timestamp
    // char dt[32] = "";
    // struct tm timestamp = _time.getTimeStruct();
    // strftime(dt,
    //          32,
    //          date_format,
    //          &timestamp);

    // char ts[40] = "";
    // sprintf(ts, "%s.%03d", dt, _time.getMillis());
    // Serial.println(ts);

    // // prepare message
    // va_list args;
    // va_start(args, msg_format);

    // char msg[128];
    // vsprintf(msg, msg_format, args);
    // va_end(args);

    // // send to streams
    // _stream(_module_name,
    //         _log_level,
    //         msg,
    //         ts);
}

void Logger::addStream(stream s)
{
    streams.push_back(s);
}

// void Logger::setDateFormat(const char *format)
// {
//     memset(date_format, 0, 16);
//     memcpy(date_format, format, 15);
// }

void Logger::_stream(const char *module_name,
                     const char *log_level,
                     const char *msg,
                     const char *timestamp)
{
    for (stream s : streams)
    {
        s(module_name,
          log_level,
          msg,
          timestamp);
    }
}