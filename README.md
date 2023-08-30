Control software
========================================
The purpose of the Control Software is to control the production line automatically or based on commands from the UI.
The goal was to run the production line without breaking the hardware and optimize output.
We use shared memory to communicate with the simulated hardware and MQTT to communicate with the UI.
## Content
* [Technologies](#technologies)
* [Build and Prerequisites](#build-and-prerequisites)
  + [Install prerequisites for Paho](#install-prerequisites-for-paho)
  + [Build and install Paho C library using cmake (anywhere on computer)](#build-and-install-paho-c-library-using-cmake--anywhere-on-computer)
  + [Build and install Paho C++ library using cmake (anywhere on computer)](#build-and-install-paho-c-library-using-cmake-anywhere-on-computer-1)
* [Run Locally](#run-locally)
  + [Clone the project](#clone-the-project)
  + [Run](#run)
* [Contributors](#contributors)
## Technologies
- Control software is written in C++
- The program receives data from the conveyor via shared memory, which is used in automation and sent to the UI
- Messages to and from the UI is parsed as JSON using nlohmann's JSON library
- MQTT is used as the communications protocol with the UI
- Eclipse Paho C++ MQTT library is used
- Project is built with CMake
- Unit testing provided by doctest
- Version control (Git) and project management (GitHub Projects)

## Build and Prerequisites
### Install prerequisites for Paho
```bash
sudo apt-get install build-essential gcc make cmake cmake-gui  cmake-curses-gui libssl-dev
```
### Build and install Paho C library using cmake (anywhere on computer)
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
### Build and install Paho C++ library using cmake (anywhere on computer)
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
## Contributors
- [Anna Pikkarainen](https://github.com/An-Man)
- [Mikko Kiviniemi](https://github.com/mikkokiviniemi)
- [Oskar Siegfrids](https://github.com/soSkary)
