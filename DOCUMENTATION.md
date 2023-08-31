
# Project documentation 🗎

## General description

The program simulates a control software / firmware of a production line that includes e.g. a conveyor belt, 3 heaters, a cooling system, 10 temperature sensors and a quality assurance camera. The software receives simulated sensor data from a [simulator](https://github.com/kmoilane/graduation_project) created by another team.
The sensor input data is used in the automatic control of the system and translated to human-readable form for the [UI](https://github.com/heidikrisse/ProductionLineUi). The automatic control system can be overriden by manual control of the parts if the user requests it through control input data, which is received over MQTT.

### Input data from sensors

The software receives input data from the simulation. The raw sensor data includes the speed of conveyor as uint8_t (0-255) representing 255 at full speed of 600 units produced per minute. The QA camera is received as a uint16_t bitmask representing 0-16 failed units. The temperature sensor readings are int16_t values representing millivolts. Our control software adds a time stamp to the received data.

### Input data from UI

The control data from UI is received as a JSON string and parsed into the control_data struct.

### Output data to UI

The translated sensor data and current manual/automatic toggle settings are made into a JSON object with nlohmann's JSON for C++ library (<https://json.nlohmann.me/>). The data includes conveyor speed in units per minute (integer value), booleans for every heater and the cooler (on/off) and booleans for their manual toggle, 10 readings (float) of temperature sensors in an array, number of QA camera failed units as an integer, and a time stamp (string).

### Output data to sensors

Control data received from UI as a string/JSON is converted and given to the sensor simulation through shared memory, and includes conveyor target speed (uint8_t), control toggle of QA camera (uint8_t), toggles for the heaters (one uint8_t) and the cooler (uint8_t). Also, there is a single uint8_t for the shutdown signal of the simulator.

### Data conversion to/from JSON

The conversion functions defined in json_output.cpp convert raw sensor data of the QA camera (uint16_t bitmask) and conveyor speed into a JSON by checking the bitmask values of the camera against a flag, and by converting the conveyor speed from an eight bit unsigned integer value to units per minute.

The control data from the UI is parsed into a control data struct object. Here the opposite conversion is done, from units per minute to an eight bit unsigned integer value.

The JSON object from the UI will follow format:
{
    {"speed_of_conveyor", integer value from 0 - 600},
    {"heater_1", boolean },
    {"heater_2", boolean },
    {"heater_3", boolean },
    {"cooler", boolean },
    {"qc_camera_toggle", boolean },
    {"conveyor_manual_control", boolean},
    {"heater_1_manual_control", boolean},
    {"heater_2_manual_control", boolean},
    {"heater_3_manual_control", boolean},
    {"cooler_manual_control", boolean}
}

The JSON object output to the UI will follow format:
{
    {"speed_of_conveyor", integer value from 0 - 600},
    {"heater_1", boolean },
    {"heater_2", boolean },
    {"heater_3", boolean },
    {"cooler", boolean },
    {"temp_sensors", { float, float, float, float, float, float, float, float, float, float }},
    {"qc_camera_fails", integer value from 0 - 16},
    {"time_stamp", "yyyy-mm-ddThh:mm:ssGMT+2"},
    {"failures", string}
}

### Shared memory interface with convoyer

Shared memory is accessed by associated file from root folder (source folder) in the path "Sim/simulation_shm". This file association is currently shared with the simulation. This can be changed in the programs main function.

The attachment, detachment and destroying of the shared memory-block is done via a RAII-style wrapper-class. This means that the class itself guarantees the proper allocation and deallocation of the shared memory space. The exit signal is shared between the processes, so that the control-software will give the signal and the simulation will exit when receiving the signal. The currently used mapping of the memory-block is bytes 0 - 27:

/***************SHARED MEMORY SCHEMA**************************

* SHM_ptr       = temperature 1         (2 bytes)   (input)  *
* SHM_ptr + 2   = temperature 2         (2 bytes)   (input)  *
* SHM_ptr + 4   = temperature 3         (2 bytes)   (input)  *
* SHM_ptr + 6   = temperature 4         (2 bytes)   (input)  *
* SHM_ptr + 8   = temperature 5         (2 bytes)   (input)  *
* SHM_ptr + 10  = temperature 6         (2 bytes)   (input)  *
* SHM_ptr + 12  = temperature 7         (2 bytes)   (input)  *
* SHM_ptr + 14  = temperature 8         (2 bytes)   (input)  *
* SHM_ptr + 16  = temperature 9         (2 bytes)   (input)  *
* SHM_ptr + 18  = temperature 10        (2 bytes)   (input)  *
* SHM_ptr + 20  = conveyor speed sensor (1 byte)    (input)  *
* SHM_ptr + 21  = conveyor speed target (1 byte)    (output) *
* SHM_ptr + 22  = heaters control       (1 byte)    (output) *
* SHM_ptr + 23  = cooler control        (1 byte)    (output) *
* SHM_ptr + 24  = camera control        (1 byte)    (output) *
* SHM_ptr + 25  = camera data           (2 bytes)   (input)  *
* SHM_ptr + 27  = exit signal           (1 byte)    (output) *
*************************************************************/

### Automatic (and manual) control

Goals for automatic control:
-Tries to maximize product output, while causing nothing to break
-Allows toggling manual overrides to any controls
-Simple internal logic using sensor data and user controls

Changes made to control data according to automation:
-Conveyor target speed is OPTIMAL_SOC. currently if temperature is over 80, target speed is 0
-Heaters are on according to heater_period (20s on -> 10s off <-> 5s on)
-Cooler is on according to setted COOLING_TEMP (currently 60)
-heater_periods, OPTIMAL_SOC, COOLING_TEMP can be changed from automatic_controls.hpp

Manual control:
-If manual control is setted on. Simply keep the manual settings on and in those parts where there are no manual settings, continue with automatic.

### MQTT interface with UI

The program uses an asynchronous MQTT client defined in the "mqtt_client" library (a wrapper class of the Paho eclipse C++ MQTT client). The client uses tcp and connects to the free mosquitto test broker (test.mosquitto.org:1883) by default. The client subscribes to the topic used by UI to receive control data from UI. When a message with the topic is published to the broker, an automatic callback calls a function to parse the message into an "input control data" JSON. The same client uses another topic to publish sensor data, translated to a human readable form, to the UI. The MQTT payloads are in string format.

Default MQTT parameters of control software client:
broker address = "tcp://test.mosquitto.org:1883"
user_id = "control_sw"
publish to topic = "sensor_control_data1"
subsribe to topic = "conveyor_params"

## Technologies

C++, CMake, MQTT, JSON, Shared memory, doctest, Git

## Prerequisites

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

### Build and install Paho C++ library (after installing the C library!) using cmake (anywhere on computer)

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

### Clone the project

```bash
git clone git@github.com:mikkokiviniemi/Control-software.git
```

### Build

```bash
cd Control-software
cmake -S . -B build
make -C build
```

### Run

```bash
chmod +x run.sh
```

```bash
./run.sh
```
