#ifndef WaterLevel_h
#define WaterLevel_h

#include "../Events/Events.h"
#include "../Sensors/Sensor.h"

#ifdef UNIT_TEST
    #include "ArduinoFake.h"
#else
    #include "Arduino.h"
#endif

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

			const char *name);

		virtual float makeReading();
		// void setup();

		Events::EventType checkTrigger(float reading);
		Events::EventType defaultHandler(float reading);
		Events::EventType constantLevelHandler(float reading);

		bool keep_water_level = false;

	private:
		uint8_t _echo;
		uint8_t _trig;
	};
} // namespace Sensor
#endif