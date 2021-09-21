#include "Programs.h"

void Programs::Program::start()
{
    if (_is_active)
    {
        return;
    }

    _relay->turnOn();
}

void Programs::Program::end()
{
    _relay->turnOff();
}

bool Programs::Program::isActive()
{
    return _is_active;
}

void Programs::Program::reactForEvent(Events::EventType event)
{
    if (_state == event)
    {
        return;
    }

    // TODO
}
