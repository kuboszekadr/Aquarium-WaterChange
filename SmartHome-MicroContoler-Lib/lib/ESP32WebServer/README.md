# ESP32WebServer

This module is responsible for handling requests that come to the device:
- config / GET | POST - to get or update requested device config file (file name as parameter)
- time / GET - to get device time
- relay / GET - to get relay current status
- reading / GET - to get lastest device reading

# Example requests and response

## `GET` config

### request
{{device_ip}}/config?api

### response 
#TODO

## `GET` reading
{{device_ip}}/reading

### response
#TODO

## `GET` relays current state
{{device_ip}}/relay

### response
#TODO
