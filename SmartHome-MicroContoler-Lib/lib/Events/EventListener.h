#ifndef EventListener_h
#define EventListener_h

#include "EventType.h"

namespace Events
{
    class EventListener
    {
    public:
        void listenTo(EventType event);
        virtual void reactForEvent(EventType event) = 0;
    };

    class Event
    {
    public:
        static Event *getEvent(EventType event);
        void addListener(EventListener *listener);
    };    
}

#endif