#include <ArduinoFake.h>
#include <unity.h>

#include "Relay.h"

using namespace fakeit;

void test_relay(void)
{
    TEST_ASSERT_EQUAL(Relay::relays.size(), 0);
    Relay *relay = new Relay("test", 0);
    TEST_ASSERT_EQUAL(Relay::relays.size(), 1);
    delete relay;
    TEST_ASSERT_EQUAL(Relay::relays.size(), 0);
}

void test_turnOn()
{
    Relay *relay = new Relay("test", 0);
    relay->turnOn();
    Verify(Method(ArduinoFake(), digitalWrite).Using(0, LOW)).Once();
    delete relay;

}

void setup()
{
    When(Method(ArduinoFake(), pinMode)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalWrite)).AlwaysReturn();
    When(Method(ArduinoFake(), digitalRead).Using(0)).AlwaysReturn(LOW);
    When(Method(ArduinoFake(), digitalRead).Using(1)).AlwaysReturn(LOW);
}

int main(int argc, char **argv)
{
    setup();
    UNITY_BEGIN();

    RUN_TEST(test_relay);
    RUN_TEST(test_turnOn);

    return UNITY_END();
}