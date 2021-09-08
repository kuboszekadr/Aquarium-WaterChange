#ifndef EventType_h
#define EventType_h

namespace Events
{
    enum EventType
    {
        EMPTY = 0,
        RELAY_STATE_CHANGE,

        WATER_LOW,
        WATER_HIGH,

        TEMP_LOW,
        TEMP_HIGH,

        PH_LOW,
        PH_HIGH
    };
}

#endif