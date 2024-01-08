#include "Event.h"

Events::Event::Event(EventType event)
{
    // TODO: check if event has been init
    _events[event] = this; // place event pointer in the array of events
    _type = event;         // assing event type
}

void Events::Event::addListener(EventListener *listener)
{
    _listeners[_listeners_amount++] = listener;
}

void Events::Event::notifyListeners()
{
    // loop through all event subscribes
    for (int i = 0; i < _listeners_amount; i++)
    {
        _listeners[i]->reactForEvent(_type);
    }
}

Events::Event *Events::Event::getEvent(EventType event)
{
    if (_events[event] == nullptr)
    {
        new Event(event);
    }
    return _events[event];
} 