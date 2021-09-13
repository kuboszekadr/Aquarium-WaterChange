#ifndef WaterLevel_h
#define WaterLevel_h

#include "Sensor.h"
#include <Arduino.h>

namespace Sensors
{
	class WaterLevel : public Sensor
	{
	public:
		WaterLevel(
			uint8_t echo,
			uint8_t trig,

			uint8_t id_sensor,
			Measures *id_measure,
			const char *name,

			float trigger_value_low,
			float trigger_value_high,

			uint32_t sampling_interval,
			uint8_t sampling_amount,

			Events::EventType trigger_low,
			Events::EventType trigger_high);

		bool makeReading();
		Events::EventType checkTrigger();

	private:
		uint8_t _echo;
		uint8_t _trig;
	};
} // namespace Sensor
#endif