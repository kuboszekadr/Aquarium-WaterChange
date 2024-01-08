#include <Arduino.h>
#include <unity.h>

#include "Events.h"
#include "EventType.h"

#include "Programs.h"
#include "WaterManager.h"
// #include "ConfigFake.hpp"

using namespace fakeit;
using namespace Programs;
using namespace Events;

WaterManager water_change = WaterManager(
    0,
    1);

void test_state(void)
{
    TEST_ASSERT_EQUAL(
        WaterManager::State::IDLE, 
        water_change.state()
        );
}

void test_react_for_event(void)
{
    EventType water_low = EventType::WATER_LOW;
    water_change.reactForEvent(water_low);
    TEST_ASSERT_EQUAL(
        WaterManager::State::POURING, 
        water_change.state()
        );

    EventType water_high = EventType::WATER_HIGH;
    water_change.reactForEvent(water_high);
    TEST_ASSERT_EQUAL(
        WaterManager::State::IDLE, 
        water_change.state()
        );
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_state);
    RUN_TEST(test_react_for_event);

    return UNITY_END();
}