#ifndef WaterChange_h
#define WaterChange_h

#include "../SmartHome-MicroContoler-Lib/Programs/Programs.h"
#include "../SmartHome-MicroContoler-Lib/Notification/Notification.h"
#include "../SmartHome-MicroContoler-Lib/Logger/Logger.h"
#include "../SmartHome-MicroContoler-Lib/Config/Config.h"

namespace Programs
{
    class WaterManager : public Program
    {
    public:
        WaterManager(uint8_t pin_pomp, uint8_t pin_water, uint8_t id);

        void start();
        void stop();

        void reactForEvent(Events::EventType event);
        void constantLevelHandler(Events::EventType event);
        void defaultHandler(Events::EventType event);

        void configure(uint8_t pin_pomp, uint8_t pin_water);

        void changeMode(bool keep_constant_level);
        void loadConfig();
        void saveConfig();

        bool isConstantLevel() { return _keep_water_level; }

    private:
        Logger logger = Logger("WaterManager");
        Relay *_pomp;
        Relay *_water;

        bool _keep_water_level = false;

        void pumpOut();
        void pour();
    };

    extern WaterManager water_change;

}
#endif