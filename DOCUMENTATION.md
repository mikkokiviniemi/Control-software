
# Project documentation 🗎

## General description
The program simulates a control software / firmware of a production line that includes e.g. a conveyor belt, 3 heaters, a cooling system, 10 temperature sensors and a quality assurance camera. The software receives simulated sensor data from a simulator created by another team (link).
The sensor input data is used in the automatic control of the system and translated to human-readable form for the UI (link). The automatic control system can be overriden by manual control of the parts if the user requests it through control input data, which is received over MQTT.
### Input data from sensors
The software receives input data from the simulation. The raw sensor data includes the speed of conveyor as uint8_t (0-255) representing 255 at full speed of 600 units produced per minute. The QA camera is received as a uint16_t bitmask representing 0-16 failed units. The temperature sensor readings are int16_t values representing millivolts. Our control software adds a time stamp to the received data.
### Input data from UI
The control data from UI is received as a JSON string and parsed into the control_data struct.
### Output data to UI
The translated sensor data and current manual/automatic toggle settings are made into a JSON object with nlohmann's JSON for C++ library (https://json.nlohmann.me/). The data includes conveyor speed in units per minute (integer value), booleans for every heater and the cooler (on/off) and booleans for their manual toggle, 10 readings (float) of temperature sensors in an array, number of QA camera failed units as an integer, and a time stamp (string).
### Output data to sensors
Control data received from UI as a string/JSON is converted and given to the sensor simulation through shared memory, and includes conveyor target speed (uint8_t), control toggle of QA camera (uint8_t), toggles for the heaters (one uint8_t) and the cooler (uint8_t). Also, there is a single uint8_t for the shutdown signal of the simulator.
### Shared memory interface with convoyer
TEXT
### Automatic (and manual) control
Goals for automatic control:
- Tries to maximize product output, while causing nothing to break
- Allows toggling manual overrides to any controls
- Simple internal logic using sensor data and user controls 

Changes made to control data according to automation:
- Conveyor target speed is OPTIMAL_SOC. currently if temperature is over 80, target speed is 0
- Heaters are on according to heater_period (20s on -> 10s off <-> 5s on)
- Cooler is on according to setted COOLING_TEMP (currently 60)
- heater_periods, OPTIMAL_SOC, COOLING_TEMP can be changed from automatic_controls.hpp

Manual control:
- If manual control is setted on. Simply keep the manual settings on and in those parts where there are no manual settings, continue with automatic.
### MQTT interface with UI
The program uses an asynchronous MQTT client defined in the "mqtt_client" library (a wrapper class of the Paho eclipse C++ MQTT client). The client uses tcp and connects to the free mosquitto test broker (test.mosquitto.org:1883) by default. The client subscribes to the topic used by UI to receive control data from UI. When a message with the topic is published to the broker, an automatic callback calls a function to parse the message into an "input control data" JSON. The same client uses another topic to publish sensor data, translated to a human readable form, to the UI. The MQTT payloads are in string format.

## Technologies
C++, CMake, MQTT, JSON, Shared memory, doctest, Git
## Prerequisites
### Install prerequisites for Paho
```bash
sudo apt-get install build-essential gcc make cmake cmake-gui  cmake-curses-gui libssl-dev
```
### Build and install Paho C library using cmake (anywhere on computer):
```bash
git clone https://github.com/eclipse/paho.mqtt.c.git
```
```bash
cd paho.mqtt.c
```
```bash
git checkout v1.3.8
```
```bash
cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
```
```bash
sudo cmake --build build/ --target install
```
```bash
sudo ldconfig
```
### Build and install Paho C++ library (after installing the C library!) using cmake (anywhere on computer):
```bash
git clone https://github.com/eclipse/paho.mqtt.cpp
```
```bash
cd paho.mqtt.cpp
```
```bash
cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON
```
```bash
sudo cmake --build build/ --target install
```
```bash
sudo ldconfig
```
## Run Locally
### Clone the project
```bash
git clone git@github.com:mikkokiviniemi/Control-software.git
```
```bash
cd Control-software
cmake -S . -B build
make -C build
```
### Run
```bash
build/main
```
