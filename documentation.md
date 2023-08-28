
# Project documentation 🗎

## Description
Describe outputs of your program accurately, knowing what kind of data your program produces is critical.
Documentation can be quite technical, since it is not going to be facing end-users but other programmers.
### Shared memory interface with convoyer
TEXT
### Automatic (and manual) control
TEXT
### MQTT interface with UI
TEXT

## Technologies
C++, CMake, MQTT, JSON, Shared memory, doctest, Git
## Prerequisites
### Install prerequisites for Paho
```bash
sudo apt-get install build-essential gcc make cmake cmake-gui \ cmake-curses-gui libssl-dev
```
### Build and install Paho C library using cmake (anywhere on computer):
```bash
git clone https://github.com/eclipse/paho.mqtt.c.git $ cd paho.mqtt.c $ git checkout v1.3.8
```
```bash
cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON
-DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON $ sudo cmake --build build/ --target install
```
```bash
sudo ldconfig
```
### Build and install Paho C++ library using cmake (anywhere on computer):
```bash
git clone https://github.com/eclipse/paho.mqtt.cpp cd paho.mqtt.cpp
```
```bash
cd paho.mqtt.cpp
```
```bash
cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON $ sudo cmake --build build/ --target install
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
