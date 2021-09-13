#ifndef Sensor_Trigger_h
#define Sensor_Trigger_h

#include "EventType.h"

namespace Sensors
{
    class Trigger
    {
    public:
        float value_low;
        float value_high;

        Events::EventType on_low;
        Events::EventType on_high;

    private:
        Events::EventType __last_event;
    }
}

#endif