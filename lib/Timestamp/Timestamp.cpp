#include "Timestamp.h"

char *getTimestamp()
{
    // get current system timestamp
    time_t now;
    time(&now);

    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

    static char ts[40];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &timeinfo);

    return ts;
}

void setTime(long epoch, const char* tz)
{
#ifdef ARDUINO
    // Copied from ESP32Time.cpp 
    struct timeval tv;
    if (epoch > 2082758399)
    {
        tv.tv_sec = epoch - 2082758399; // epoch time (seconds)
    }
    else
    {
        tv.tv_sec = epoch; // epoch time (seconds)
    }
    tv.tv_usec = 0; // microseconds
    settimeofday(&tv, NULL);
    setenv("TZ", tz, 0);
    tzset();
#endif
}