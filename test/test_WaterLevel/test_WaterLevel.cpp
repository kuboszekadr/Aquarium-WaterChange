#include <ArduinoFake.h>
#include <unity.h>

#include "WaterLevel.h"
#include "Event.h"
#include "EventType.h"

using namespace Sensors;
using namespace Events;
using namespace fakeit;

WaterLevel *water_level = nullptr;

void test_default_handler()
{
    // Empty event- between thresholds
    water_level->setTriggerValues(25.0, 20.0);
    water_level->keep_water_level = false;

    EventType event = water_level->defaultHandler(17);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    // Water Low 
    event = water_level->defaultHandler(26);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_LOW, event);

    // Water high
    event = water_level->defaultHandler(2);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    // IDLE
    event = water_level->defaultHandler(22);
    TEST_ASSERT_EQUAL(Events::EventType::EMPTY, event);
}

void test_content_level_handler()
{
    // Empty event- between thresholds
    water_level->setTriggerValues(25.0, 20.0);
    water_level->keep_water_level = true;

    EventType event = water_level->constantLevelHandler(17);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    // Water Low 
    event = water_level->constantLevelHandler(26);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_LOW, event);

    // Water high
    event = water_level->constantLevelHandler(2);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    // IDLE
    event = water_level->defaultHandler(22);
    TEST_ASSERT_EQUAL(Events::EventType::EMPTY, event);

    water_level->keep_water_level = false;
}

void test_check_trigger()
{
    water_level->setTriggerValues(25.0, 20.0);

    EventType event = water_level->checkTrigger(999);
    TEST_ASSERT_EQUAL(Events::EventType::READING_ERROR, event);

    event = water_level->checkTrigger(0);
    TEST_ASSERT_EQUAL(Events::EventType::READING_ERROR, event);

    event = water_level->checkTrigger(-1);
    TEST_ASSERT_EQUAL(Events::EventType::READING_ERROR, event);

    event = water_level->defaultHandler(17);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);  
}

void test_water_change()
{
    water_level->setTriggerValues(25.0, 20.0);
    water_level->keep_water_level = false;

    EventType event = water_level->checkTrigger(17);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    //Pomping out
    event = water_level->checkTrigger(18);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    event = water_level->checkTrigger(19);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    event = water_level->checkTrigger(22);
    TEST_ASSERT_EQUAL(Events::EventType::EMPTY, event);

    event = water_level->checkTrigger(25);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_LOW, event);

    event = water_level->checkTrigger(26);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_LOW, event);

    // Refill
    event = water_level->checkTrigger(24);
    TEST_ASSERT_EQUAL(Events::EventType::EMPTY, event);

    event = water_level->checkTrigger(24);
    TEST_ASSERT_EQUAL(Events::EventType::EMPTY, event);

    event = water_level->checkTrigger(20);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    event = water_level->checkTrigger(19);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);
}

void test_water_constant_level()
{
    water_level->setTriggerValues(25.0, 20.0);
    water_level->keep_water_level = true;

    EventType event = water_level->checkTrigger(17);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    event = water_level->checkTrigger(18);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    event = water_level->checkTrigger(19);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_HIGH, event);

    event = water_level->checkTrigger(22);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_LOW, event);

    event = water_level->checkTrigger(21);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_LOW, event);

    event = water_level->checkTrigger(20);
    TEST_ASSERT_EQUAL(Events::EventType::WATER_LOW, event);
}

void setup()
{
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();

    water_level = new WaterLevel(1, 2, "WaterLevelTests");
}

int main(int argc, char **argv)
{
    setup();

    UNITY_BEGIN();

    RUN_TEST(test_default_handler);
    RUN_TEST(test_content_level_handler);
    RUN_TEST(test_check_trigger);
    RUN_TEST(test_water_change);
    RUN_TEST(test_water_constant_level);

    return UNITY_END();
}