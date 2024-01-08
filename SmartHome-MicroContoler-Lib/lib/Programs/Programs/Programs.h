#ifndef Programs_h
#define Programs_h

#include "Events/Events.h"
#include "Relay/Relay.h"

#ifdef UNIT_TEST
    #include ArduinoFake
#else
    #include <Arduino.h>
#endif

namespace Programs
{
    class Program : public Events::EventListener
    {
    public:
        Program(){};

        virtual void start();
        virtual void stop();
        virtual void reactForEvent(Events::EventType event);

        void deactivate();
        void activate();

        bool isActive();

    protected:
        uint8_t _id;
        uint32_t _execution_id = 0;
        bool _is_active = false;
        Events::EventType _event = Events::EventType::EMPTY;

    private:
        Relay *_relay;
    };
} // namespace Programs
#endif