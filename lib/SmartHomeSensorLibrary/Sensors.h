#ifndef Sensors_h
#define Sensors_h

#include "Events.h"
#include "Measures.h"
#include "Sensor.h"

#include <Arduino.h>

#define SENSOR_THRESHOLD_MIN 0.0
#define SENSOR_THRESHOLD_MAX 99.99
#define SENSOR_JSON_SIZE 50

namespace Sensors
{
  extern Sensor *sensors[SENSOR_AMOUNT]; // array of generated sensors
  extern uint8_t sensors_amount;         // how many sensors are initalized

  void loop();
} // namespace Sensors
#endif