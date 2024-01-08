#include "Events.h"

int Events::queue_length = 0;
Events::EventType Events::queue[EVENT_QUEUE_LENGTH] = {};
Events::Event *Events::Event::_events[EVENT_DEFINED_EVENTS];

void Events::raise(EventType event)
{
    if (queue_length >= EVENT_QUEUE_LENGTH)
    {
        return;
    }
    else if (event == EMPTY)
    {
        return;
    }
    queue[queue_length++] = event; // insert event into queue
}

void Events::notifyListeners()
{
    for (int i = 0; i < queue_length; i++)
    {
        EventType event_type = queue[i];
        Event *event = Event::getEvent(event_type);

        if (event)
        {
            event->notifyListeners();
        }
        queue[i] = EventType::EMPTY;
    }
    queue_length = 0; // restart queue
}
