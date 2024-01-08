#ifndef Sensors_h
#define Sensors_h

#include "Measures.h"
#include "Sensor.h"

#include "../Events/Events.h"

#include <Arduino.h>
#include <ArduinoJson.h>

#define SENSOR_THRESHOLD_MIN 0.0
#define SENSOR_THRESHOLD_MAX 99.99
#define SENSOR_JSON_SIZE 50
#define SENSOR_AMOUNT 10 // maximum amount of sensors

namespace Sensors
{
  extern uint8_t sensors_amount;         // how many sensors are initalized
  extern Sensor *sensors[SENSOR_AMOUNT]; // array of generated sensors
  extern StaticJsonDocument<1024> readings;

  void loop();
} // namespace Sensors
#endif