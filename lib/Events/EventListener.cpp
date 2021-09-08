#include "EventListener.h"

void Events::EventListener::listenTo(EventType event)
{
    Event *_event = Event::getEvent(event);
    _event->addListener(this); // add event listener
}
