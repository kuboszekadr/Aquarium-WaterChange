#include <ArduinoFake.h>
#include <unity.h>

#include "Events.h"
#include "EventType.h"

#include "Programs.h"
#include "WaterManager.h"

using namespace Programs;
using namespace Events;
using namespace fakeit;

WaterManager *water_manager = nullptr;

void test_state(void)
{
    TEST_ASSERT_EQUAL(
        WaterManager::State::IDLE, 
        water_manager->state()
        );
}

void test_react_for_event(void)
{
    EventType water_low = EventType::WATER_LOW;
    water_manager->reactForEvent(water_low);
    TEST_ASSERT_EQUAL(
        WaterManager::State::POURING, 
        water_manager->state()
        );

    EventType water_high = EventType::WATER_HIGH;
    water_manager->reactForEvent(water_high);
    TEST_ASSERT_EQUAL(
        WaterManager::State::IDLE, 
        water_manager->state()
        );
}

void test_relays(void)
{
    TEST_ASSERT_EQUAL(Relay::relays.size(), 2);
}

void setup()
{
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalRead).Using(0)).AlwaysReturn(LOW);
    When(Method(ArduinoFake(), digitalRead).Using(1)).AlwaysReturn(LOW);

    water_manager = new WaterManager(0, 1);
}

int main(int argc, char **argv)
{
    setup();
    UNITY_BEGIN();

    RUN_TEST(test_state);
    RUN_TEST(test_react_for_event);

    delete water_manager;
    return UNITY_END();
}