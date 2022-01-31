# About

This repo contains source code which I am developing to automate water change in my aquarium. At the moment it is deployed on ESP32, which is controlling pomp (pomps water out) and electrovalve for water in. 

Also HC-SR04 sensor is being used in order to get water level. Please note that I have sump, so sensor is mounted above the water level.

## Features
* web page with configuration panel,
* integration with my smarthome system (data stream),
* daily schedule for water change,

## TODOs
* Create daily-activation schedule,
* Create logs and show them on page,
* Configure sensor via http request,
* Update sensor page and show current status,
* water change on-demand.

# Project structure

* `board` - contains board breadboard-prototype in `Fritzing`,
* `lib` - contains custom libraries used in project,
* `src` - contains `main.cpp` file (`PlatformIO` legacy).