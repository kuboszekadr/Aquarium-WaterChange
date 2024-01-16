#ifndef WaterChange_h
#define WaterChange_h

#include "Programs/Programs.h"
#include "Notification/Notification.h"
#include "Logger/Logger.h"
#include "Config/Config.h"
#include "Relay/Relay.h"

namespace Programs
{
    class WaterManager : public Program
    {
    public:
        enum State
        {
            POURING,
            POMPING,
            IDLE
        };

        WaterManager(uint8_t pin_pomp, uint8_t pin_water);
        ~WaterManager() {};

        void start();
        void stop();
        void reactForEvent(Events::EventType event);

        void constantLevelHandler(Events::EventType event);
        void defaultHandler(Events::EventType event);

        void configure(uint8_t pin_pomp, uint8_t pin_water);

        State state() { return _state; }

    private:
        Logger logger = Logger("WaterManager");
        Relay *_pomp;
        Relay *_water;

        State _state = IDLE;

        void pumpOut();
        void pour();
    };

    extern WaterManager water_change;
}
#endif