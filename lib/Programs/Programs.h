#ifndef Programs_h
#define Programs_h

#include <Events.h>
#include <Relay.h>

#include <Arduino.h>

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
        Events::EventType _state = Events::EventType::EMPTY;

    private:
        Relay *_relay;
    };
} // namespace Programs
#endif