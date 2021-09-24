#ifndef WaterLevel_h
#define WaterLevel_h

#include "Events.h"
#include "Sensor.h"
#include <Arduino.h>

#define WATER_LEVEL_MEASURE_ID 2

namespace Sensors
{
	extern Measures water_level_measure[1];
	class WaterLevel : public Sensor
	{
	public:
		WaterLevel(
			uint8_t echo,
			uint8_t trig,

			uint8_t id_sensor,
			const char *name,

			float trigger_value_low,
			float trigger_value_high,

			uint32_t sampling_interval,
			uint8_t sampling_amount);

		bool makeReading();
		Events::EventType checkTrigger();

	private:
		uint8_t _echo;
		uint8_t _trig;
	};
} // namespace Sensor
#endif