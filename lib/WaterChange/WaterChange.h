#ifndef WaterChange_h
#define WaterChange_h

#include "Programs.h"

#include <Logger.h>

namespace Programs
{
class WaterChange : public Program
{
public:
    WaterChange(uint8_t pin_pomp, uint8_t pin_water, uint8_t id);
    
    void start();
    void stop();

    void reactForEvent(Events::EventType event);

private:
    Logger logger = Logger("WaterChange");
    Relay *_pomp;
    Relay *_water;

    void pumpOut();
    void pour();
};
} // namespace Programs
#endif