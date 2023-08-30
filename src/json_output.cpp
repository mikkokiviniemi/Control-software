#include "json_output.hpp"
#include "json.hpp"
#include "data_structures.hpp"

#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>

constexpr float RAW_TO_UI{ 2.353f };
constexpr float UI_TO_RAW{ 0.425f };

int count_qc_fails(uint16_t qc_camera_fails)
{
    int count{ 0 };

    for (int i{ 15 }; i >= 0; --i)
    {
        if (qc_camera_fails & (1 << i))
        {
            ++count;
        }
    }
    return count;
}

bool check_bitmask(uint8_t bitmask, uint8_t flag)
{
    return (bitmask & flag);
}

json create_output_sensor_data(const sensor_data& sensor_input, const control_data& ctrl_data) {
    std::stringstream time;
    time << std::put_time(std::localtime(&sensor_input.time_stamp), "%FT%TGMT+2");
    json output_data = {
    {"speed_of_conveyor", conveyor_speed_raw_to_ui(sensor_input.speed_of_conveyor)},
    {"heater_1", check_bitmask(ctrl_data.heaters, HEATER_1)},
    {"heater_2", check_bitmask(ctrl_data.heaters, HEATER_2)},
    {"heater_3", check_bitmask(ctrl_data.heaters, HEATER_3)},
    {"cooler", (ctrl_data.cooler == 1)},
    {"temp_sensors", {
        temperature_raw_to_ui(sensor_input.temp_sensor01), 
        temperature_raw_to_ui(sensor_input.temp_sensor02),
        temperature_raw_to_ui(sensor_input.temp_sensor03),
        temperature_raw_to_ui(sensor_input.temp_sensor04),
        temperature_raw_to_ui(sensor_input.temp_sensor05),
        temperature_raw_to_ui(sensor_input.temp_sensor06),
        temperature_raw_to_ui(sensor_input.temp_sensor07),
        temperature_raw_to_ui(sensor_input.temp_sensor08),
        temperature_raw_to_ui(sensor_input.temp_sensor09),
        temperature_raw_to_ui(sensor_input.temp_sensor10)
        }},
    {"qc_camera_fails", count_qc_fails(sensor_input.qc_camera_fails)},
    {"time_stamp", time.str()}
    };
    return output_data;
}

//Convert Json to control_data
control_data json_to_control_data(const json& json_elem) {
    control_data new_data;
    new_data.speed_of_conveyor = conveyor_speed_ui_to_raw(json_elem["speed_of_conveyor"]);
    bool heater_1 = json_elem["heater_1"];
    bool heater_2 = json_elem["heater_2"];
    bool heater_3 = json_elem["heater_3"];
    bool cooler = json_elem["cooler"];
    new_data.heaters =  (heater_1 << 0) | (heater_2 << 1) | (heater_3 << 2);
    new_data.cooler = cooler << 0;
    new_data.camera_toggle = json_elem["qc_camera_toggle"];
    return new_data;
}

// take raw conveyor speed and convert it to units/min
int conveyor_speed_raw_to_ui(uint8_t conveyor_speed)
{
    return static_cast<int>(round(conveyor_speed * RAW_TO_UI));
}

// take conveyor speed as units/min and convert to raw
uint8_t conveyor_speed_ui_to_raw(int conveyor_speed)
{
    return static_cast<uint8_t>(round(conveyor_speed * UI_TO_RAW));
}

//take raw temperature, convert to float and round down to one decimal place
float temperature_raw_to_ui(int16_t temperature)
{
    return (static_cast<float>(temperature) * 10) / 100;
}
