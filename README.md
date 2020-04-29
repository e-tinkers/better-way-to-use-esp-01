## Better way to use ESP-01
This repo is a proof-of-concept for a better way of using ESP-01 as a WiFi shield for IoT project.

### What this is about?
There are generally two ways of using an ESP-01 module:

### Uses it as a Wifi modem
In this case you communicate with the ESP-01 via AT commands. And ESP-01 by default came with the firmware for handling the AT commands. Parsing of the AT commands is tedious and slow, although this can be overcomed by using a good library like [LiteESP8266Client](https://github.com/e-tinkers/LiteESP8266Client), however, most of the AT command libraries do not support SSL. To implment SSL connectivity is complicate if not impossible, and would requires re-flash of ESP-01 to add SSL keys. If you are using an MCU like Arduino Uno, it will simply not have sufficient memory to handle SSL key or even http response sent back from a server.

### Uses it as a stand-alone MCU like an Arduino
The ESP-01 can be used as a stand-alone MCU by flashing it with an Arduino Core (or other implementations such as micropython or RTOS). The ESP-01 however has limited hardware interface to be meaningful for any realistic project.

### Better way of using ESP-01
There is a better way to use ESP-01 with a hybrid solution, allowing a Host MCU such as an Arduino to use the ESP-01 as a WiFi shield. The ESP01 will responsible for handling all the WiFi and communication with outside world, even SSL connectivity.  The Host take care of the interface with all sensors. The host and ESP-01 communicated with each other via a simple serial interface.

[![ESP32 ADC linearity](https://github.com/e-tinkers/better-way-to-use-esp-01/blob/master/images/connection-between-host-and-esp-01.png)](https://github.com/e-tinkers/better-way-to-use-esp-01/blob/master/images/connection-between-host-and-esp-01.png)

*** Host Serial Controller ***

The Host is a simple serial state machine implementation to determine the states of the ESP-01, and executes instructions accordingly. The state machine is high customizable to handle more states based on your actual application. Both Host and ESP-01 can go into deep sleep, and a GPIO pin can be used to wake-up/reset ESP-01.

*** ESP-01 Web Client ***

A similar serial state machine is implemnted to wait for incomming message from Host, once it is received the incoming message, it established a HTTPS web client and send the incoming message as a POST request payload to a web server. Upon the completion of sending the POST request, the ESP-01 could go into deep sleep to reduce power consumption. ESP-01 sends two states to the Host, 1) when the WiFi is ready; 2) when it completed all the tasks and ready to go into sleep mode.
