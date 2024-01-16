#ifndef WaterLevel_h
#define WaterLevel_h

#include "Events/Events.h"
#include "Sensors/Sensor.h"

#ifdef ARDUINO
    #include "Arduino.h"
#else
    #include "ArduinoFake.h"
#endif

namespace Sensors
{
	class WaterLevel : public Sensor
	{
	public:
		WaterLevel(
			uint8_t echo,
			uint8_t trig,

			const char *name);

		float makeReading();

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