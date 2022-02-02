#ifndef WaterLevel_h
#define WaterLevel_h

#include "../SmartHome-MicroContoler-Lib/Events/Events.h"
#include "../SmartHome-MicroContoler-Lib/Sensors/Sensor.h"

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
			const char *name);

		bool makeReading();
		Events::EventType checkTrigger();

	private:
		uint8_t _echo;
		uint8_t _trig;
	};
} // namespace Sensor
#endif