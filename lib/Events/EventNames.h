#include <Arduino.h>

// Definition of all event labels in PROGMEM
const char event_empty_str[] PROGMEM = "EMPTY";
const char event_relay_change_str[] PROGMEM = "RELAY_STATE_CHANGE";

const char event_water_low_str[] PROGMEM = "WATER_LOW";
const char event_water_high_str[] PROGMEM = "WATER_HIGH";

const char event_temp_low_str[] PROGMEM = "TEMP_LOW";
const char event_temp_high_str[] PROGMEM = "TEMP_HIGH";

const char event_ph_low_str[] PROGMEM = "PH_LOW";
const char event_ph_high_str[] PROGMEM = "PH_HIGH";

const char *const event_type_labels[] PROGMEM = {
    // other event types
    event_empty_str,
    event_relay_change_str,

    // water level event names
    event_water_low_str,
    event_water_high_str,

    // temperature event names
    event_temp_low_str,
    event_temp_high_str,

    // ph event names
    event_ph_low_str,
    event_ph_high_str};
