#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

#include <time.h>
#include <sys/time.h>

char* getTimestamp();
void setTime(long epoch, const char* tz);

#endif