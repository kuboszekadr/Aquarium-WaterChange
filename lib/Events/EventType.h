#ifndef EventType_h
#define EventType_h

namespace Events
{
    enum EventType
    {
        EMPTY,
        RELAY_STATE_CHANGE,

        WATER_LOW,
        WATER_HIGH,

        TEMP_LOW,
        TEMP_HIGH,

        // PH_LOW,
        // PH_HIGH,
        READING_ERROR
    };
}

#endif