#include <unity.h>
#include <numeric>
#include <string>

#include "Readings.h"

using namespace Sensors;

const char *MEASURE_NAME = "TEST_MEASURE";

void test_single_measure_setup()
{
    Readings readings({MEASURE_NAME});
    int readings_size = readings.getReadings()[MEASURE_NAME].size();
    TEST_ASSERT_EQUAL(0, readings_size);
}

void test_add_new_reading()
{
    Readings readings({MEASURE_NAME});
    
    readings.addNewReading(MEASURE_NAME, 1.0);
    readings.addNewReading(MEASURE_NAME, 2.0);
    readings.addNewReading(MEASURE_NAME, 3.0);

    int readings_size = readings.getReadings()[MEASURE_NAME].size();
    TEST_ASSERT_EQUAL(3, readings_size);

    float sum = 0.0f;
    std::vector<float> data = readings.getReadings()[MEASURE_NAME];
    for (const auto &value : data) {
        sum += value;
    }

    TEST_ASSERT_EQUAL_FLOAT(6.0f, sum);
}

void test_restart()
{
    Readings readings({MEASURE_NAME});
    readings.addNewReading(MEASURE_NAME, 1.0);
    readings.addNewReading(MEASURE_NAME, 2.0);
    readings.addNewReading(MEASURE_NAME, 3.0);

    readings.restart();

    int readings_size = readings.getReadings()[MEASURE_NAME].size();
    TEST_ASSERT_EQUAL(0, readings_size);
}

void test_toJson()
{
    Readings readings({MEASURE_NAME});
    readings.addNewReading(MEASURE_NAME, 1.0);
    readings.addNewReading(MEASURE_NAME, 2.0);
    readings.addNewReading(MEASURE_NAME, 3.0);

    JsonDocument doc = readings.toJSON();
    TEST_ASSERT_EQUAL(1, doc.size());

    JsonObject obj = doc[0];
    // keys check
    TEST_ASSERT_TRUE(obj.containsKey("timestamp"));
    TEST_ASSERT_TRUE(obj.containsKey("value"));
    TEST_ASSERT_TRUE(obj.containsKey("measure_name"));

    // value check
    TEST_ASSERT_EQUAL_FLOAT(2.0f, obj["value"].as<float>());

    // timestamp check
    std::string result = obj["timestamp"].as<std::string>();
    std::string expected = "20200101 000000";
    
    bool test = result == expected;
    TEST_ASSERT_TRUE(test);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_single_measure_setup);
    RUN_TEST(test_add_new_reading);
    RUN_TEST(test_restart);
    RUN_TEST(test_toJson);

    return UNITY_END();
}
