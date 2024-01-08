#ifndef Notification__h
#define Notification__h

#include <vector>

namespace Notification
{
    typedef void (*stream)(
        const char *title,
        const char *message);

    void push(const char *title, const char *message);
    void addStream(stream s);

    extern std::vector<stream> streams;
}

#endif