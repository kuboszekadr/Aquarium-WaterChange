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

    void changeMode();
    void loadConfig();
    void saveConfig();

private:
    Logger logger = Logger("WaterManager");
    Relay *_pomp;
    Relay *_water;

    bool constant_level = false;

    void pumpOut();
    void pour();
};
} // namespace Programs
#endif