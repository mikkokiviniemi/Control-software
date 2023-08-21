#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H
#include "external/json.hpp"
#include "external/json_fwd.hpp"
#include <array>

using json = nlohmann::json;

std::array<float, 16> temp_sensors; // tämä arrayksi voi olla mahdollinen

json output_data = {
    {"speed_of_conveyor", 0}, // uint16_t 
    {"heater_1", false},
    {"heater_2", false},
    {"heater_3", false},
    {"cooler", false},
    {"qc_camera_status", false},
    {"temp_sensors", {0.0f, 0.0f, 0.0f}}, // tai sitten näin mutta 10 kpl
    //...
};

/*
JSON Output:
{
    "speed_of_conveyor" : int,
    "heater_1": bool,
    "heater_2": bool,
    "heater_3": bool,
    "cooler": bool,
    "qc_camera_status": bool,
    "temp_sensor01": float,
    "temp_sensor02": float,
    "temp_sensor03": float,
    "temp_sensor04": float,
    "temp_sensor05": float,
    "temp_sensor06": float,
    "temp_sensor07": float,
    "temp_sensor08": float,
    "temp_sensor09": float,
    "temp_sensor10": float,
    "time_stamp": value
}

/*
JSON Output_2:

{
    "qc_camera_fails": ???,
    "time_stamp": value
}
*/

/*
JSON Commands:
{
    "speed_of_conveyor" : int,
    "heater_1": bool,
    "heater_2": bool,
    "heater_3": bool,
    "cooler": bool,
    "qc_camera_status": bool,
}
*/

#endif