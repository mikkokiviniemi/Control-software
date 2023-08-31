
# Project documentation 🗎

## Description
Describe outputs of your program accurately, knowing what kind of data your program produces is critical.
Documentation can be quite technical, since it is not going to be facing end-users but other programmers.
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
TEXT

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
### Build and install Paho C++ library using cmake (anywhere on computer):
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
