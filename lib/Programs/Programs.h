#ifndef Programs_h
#define Programs_h

#include "../Events/Events.h"

namespace Programs
{
    class Program : public Events::EventListener
    {
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void reactForEvent(Events::EventType event) = 0;

        void deactivate();
        void activate();

        bool isActive();

    protected:
        bool _is_active = false;
        Events::EventType _event = Events::EventType::EMPTY;
    };
} // namespace Programs
#endif