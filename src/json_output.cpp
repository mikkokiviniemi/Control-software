#include "json_output.hpp"
#include "json.hpp"
#include "data_structures.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>

bool check_bitmask(uint8_t bitmask, uint8_t flag)
{
    return (bitmask & flag);
}

json create_output_sensor_data(const sensor_data& sensor_input, const control_data& ctrl_data) {
    std::stringstream time;
    time << std::put_time(std::localtime(&sensor_input.time_stamp), "%FT%TGMT+2");
    json output_data = {
    {"speed_of_conveyor", static_cast<int>(sensor_input.speed_of_conveyor)},
    {"heater_1", check_bitmask(ctrl_data.heaters, HEATER_1)},
    {"heater_2", check_bitmask(ctrl_data.heaters, HEATER_2)},
    {"heater_3", check_bitmask(ctrl_data.heaters, HEATER_3)},
    {"cooler", (ctrl_data.cooler == 1)},
    {"qc_camera_status", check_bitmask(ctrl_data.camera_toggle, 1)},
    {"temp_sensors", {
        static_cast<float>(sensor_input.temp_sensor01) / 10.0f, 
        static_cast<float>(sensor_input.temp_sensor02) / 10.0f,
        static_cast<float>(sensor_input.temp_sensor03) / 10.0f,
        static_cast<float>(sensor_input.temp_sensor04) / 10.0f,
        static_cast<float>(sensor_input.temp_sensor05) / 10.0f,
        static_cast<float>(sensor_input.temp_sensor06) / 10.0f,
        static_cast<float>(sensor_input.temp_sensor07) / 10.0f,
        static_cast<float>(sensor_input.temp_sensor08) / 10.0f,
        static_cast<float>(sensor_input.temp_sensor09) / 10.0f,
        static_cast<float>(sensor_input.temp_sensor10) / 10.0f
        }},
    {"time_stamp", time.str()}
    };
    return output_data;
}

json create_camera_feed_output(const sensor_data& sensor_input) {
    std::stringstream time;
    time << std::put_time(std::localtime(&sensor_input.time_stamp), "%FT%TGMT+2");
    
    json camera_feed_output = {
    {"qc_camera_fails", sensor_input.qc_camera_fails},
    {"time_stamp", time.str()}
    };
    return camera_feed_output;
}


//Convert Json to control_data
control_data json_to_control_data(const json& json_elem) {
    control_data new_data;
    new_data.speed_of_conveyor = static_cast<uint8_t>(json_elem["speed_of_conveyor"]);
    bool heater_1 = json_elem["heater_1"];
    bool heater_2 = json_elem["heater_2"];
    bool heater_3 = json_elem["heater_3"];
    bool cooler = json_elem["cooler"];
    new_data.heaters =  (heater_1 << 0) | (heater_2 << 1) | (heater_3 << 2);
    new_data.cooler = cooler << 0;
    new_data.camera_toggle = json_elem["qc_camera_status"];
    return new_data;
}
