# Config

This module provides ability to read config file saved in `json` format. Please note that object is not saved in any static buffer to save device memory.

By default, configs are saved/loaded in/from `config` folder.

# Example usage

## Printing file content
```
Config sensor_config = Config("sensor"); // inits new object
sensor_config.load(); // reads json file stored under /config/sensor.json

Serial.serializeJson(sensor_config.data, Serial); // prints file content
```
