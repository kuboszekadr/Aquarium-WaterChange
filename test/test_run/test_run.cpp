#include <ArduinoFake.h>
#include <unity.h>

#include "WaterLevel.h"
#include "WaterManager.h"

#include "Event.h"
#include "EventType.h"
#include "Programs.h"

using namespace Sensors;
using namespace Programs;
using namespace Events;
using namespace fakeit;

WaterLevel *water_level = nullptr; 
WaterManager *water_manager = nullptr;

void test_water_refill_wo_contant_level()
{
    water_level->setTriggerValues(25.0, 20.0);
    water_level->keep_water_level = false;

    TEST_ASSERT_FALSE(water_manager->isActive());
    water_manager->start();

    // Pomping out
    water_level->checkTrigger(17); // Water high
    Events::notifyListeners();
    WaterManager::State state = water_manager->state();
    
    TEST_ASSERT_EQUAL(WaterManager::State::POMPING, state);
    TEST_ASSERT_TRUE(water_manager->isActive());

    water_level->checkTrigger(20); // Water high
    Events::notifyListeners();
    state = water_manager->state();
    
    TEST_ASSERT_EQUAL(WaterManager::State::POMPING, state);
    TEST_ASSERT_TRUE(water_manager->isActive());

    water_level->checkTrigger(22); // IDLE Event
    Events::notifyListeners();
    state = water_manager->state();
    
    TEST_ASSERT_EQUAL(WaterManager::State::POMPING, state);
    TEST_ASSERT_TRUE(water_manager->isActive());

    //Pouring
    water_level->checkTrigger(25); // WaterLow
    Events::notifyListeners();
    state = water_manager->state();
    
    TEST_ASSERT_EQUAL(WaterManager::State::POURING, state);
    TEST_ASSERT_TRUE(water_manager->isActive());

    water_level->checkTrigger(22); // WaterLow
    Events::notifyListeners();
    state = water_manager->state();
    
    TEST_ASSERT_EQUAL(WaterManager::State::POURING, state);
    TEST_ASSERT_TRUE(water_manager->isActive());

    water_level->checkTrigger(20); // WaterLow
    Events::notifyListeners();
    state = water_manager->state();
    
    TEST_ASSERT_EQUAL(WaterManager::State::IDLE, state);
    TEST_ASSERT_FALSE(water_manager->isActive());
}

void setup()
{
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalRead).Using(1)).AlwaysReturn(LOW);
    When(Method(ArduinoFake(), digitalRead).Using(2)).AlwaysReturn(LOW);

    water_level = new WaterLevel(1, 2, "WaterLevelTests");
    water_manager = new WaterManager(3, 4);

}


int main(int argc, char **argv)
{
    setup();
    UNITY_BEGIN();

    RUN_TEST(test_water_refill_wo_contant_level);

    return UNITY_END();
}