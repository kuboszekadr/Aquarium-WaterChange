#include <unity.h>

#include "ArduinoFake.h"
#include "Events.h"
#include "EventType.h"

using namespace fakeit;

void setUp(void)
{
    ArduinoFakeReset();
}

void tearDown(void) {
    // clean stuff up here
}

void test_event_raise(void)
{
    Events::raise(Events::EventType::WATER_LOW);
    TEST_ASSERT_EQUAL(1, Events::queue_length);

    Events::notifyListeners();
    TEST_ASSERT_EQUAL(0, Events::queue_length);    
}

void test_event_raise_multiple(void)
{
    Events::raise(Events::EventType::WATER_LOW);
    Events::raise(Events::EventType::WATER_LOW);
    Events::raise(Events::EventType::WATER_LOW);
    TEST_ASSERT_EQUAL(3, Events::queue_length);

    Events::notifyListeners();
    TEST_ASSERT_EQUAL(0, Events::queue_length);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_event_raise);
    RUN_TEST(test_event_raise_multiple);

    return UNITY_END();
}