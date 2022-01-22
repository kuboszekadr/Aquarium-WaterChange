#include "Notification.h"

std::vector<Notification::stream> Notification::streams = std::vector<Notification::stream>();

void Notification::push(const char *title, const char *message)
{
    for (stream s: streams)
    {
        s(title, message);
    }
}

void Notification::addStream(Notification::stream s)
{
    streams.push_back(s);
}