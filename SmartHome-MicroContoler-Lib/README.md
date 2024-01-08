# SmartHome-MicroController-Lib

A `C++` library of re-usable components to be used in ESP32 projects. My goal was to achive as-much-as-possible separable modules from which you can setup-up any other device.

Please note, that I am developing this in my free time, thus sometimes I prioritize _making it work_ instead _making it well_.

Each module is stored in separate folder, inside which more detailed info is also added, but here I am giving a brief description of each:

* `Config` - this module is responsible for handling config files saved in `json` format,
* `Device` - this module contains setup functions for device runned in `setup` in `ino` sketch, 
* `ESP32WebServer` - contains basic server services,
* `Events` - handles and distributes events messages (for instance `WATER_LOW`),
* `Logger` - logs message to defined streams,
* `Notification` - rises notifications,
* `Programs` - contains class to be derived to implement device-related tasks,
* `Relay` - high-level wrapper arround device pins, 
* `Sensors` - contains namespace and interface for various sensors,
* `SmartHomeDevice` - contains module that sends relevant data into external endpoint,
* `WiFiManager` - wrapper around `WiFi.h` to manage wifi connection.

# TODO's
* Add OVA (over the air) device update
* add device file struct
* add html files as templates