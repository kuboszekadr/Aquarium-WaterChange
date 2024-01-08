#ifndef Event_h
#define Event_h

#define EVENT_MAX_SUBSCRIBERS 3
#define EVENT_DEFINED_EVENTS 9

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
        Event(EventType event);
        void notifyListeners();

        static Event *getEvent(EventType event);
        void addListener(EventListener *listener);

    private:
        static Event *_events[EVENT_DEFINED_EVENTS];
        
        EventListener *_listeners[EVENT_MAX_SUBSCRIBERS];
        int _listeners_amount = 0;

        EventType _type;
    };
}

#endif