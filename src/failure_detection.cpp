#include "failure_detection.hpp"
#include "input_validation.hpp"

#include <string>

// Can only be used with static or initialized control_data structs
bool control_data_is_empty(const control_data& ctrl_data)
{
    return (ctrl_data.speed_of_conveyor == 0 &&
        ctrl_data.heaters == 0 &&
        ctrl_data.cooler == 0 &&
        ctrl_data.camera_toggle == 0);
}

// Can only be used with static or initialized sensor_data structs
bool sensor_data_is_empty(const sensor_data& sensor_input)
{
    return (sensor_input.speed_of_conveyor == 0 &&
        sensor_input.qc_camera_fails == 0 &&
        sensor_input.temp_sensor01 == 0 &&
        sensor_input.temp_sensor02 == 0 &&
        sensor_input.temp_sensor03 == 0 &&
        sensor_input.temp_sensor04 == 0 &&
        sensor_input.temp_sensor05 == 0 &&
        sensor_input.temp_sensor06 == 0 &&
        sensor_input.temp_sensor07 == 0 &&
        sensor_input.temp_sensor08 == 0 &&
        sensor_input.temp_sensor09 == 0 &&
        sensor_input.temp_sensor10 == 0 &&
        sensor_input.time_stamp == 0);
}

// check if invalid temperature sensor readings, and return a string of which are invalid
std::string failed_temp_sensors(const sensor_data& sensor_input)
{
    uint16_t failed_temps{ temperature_input_validation(sensor_input) };
    if (failed_temps == 0)
    {
        return { "" };
    }
    std::string failed_string{ "failed temperature readings: " };
    for (int i{ 0 }; i < 10; ++i)
    {
        if (failed_temps & (1 << i))
        {
            failed_string += "sensor ";
            failed_string += std::to_string(i + 1);
        }
    }
    failed_string += '\n';
    return failed_string;
}
// check if invalid control data inputs and return a string of which are invalid
std::string invalid_control_data(const control_data& ctrl_data)
{
    uint8_t failed_control_data{ control_input_validation(ctrl_data) };
    
    if (failed_control_data == 0)
    {
        return { "" };
    }
    std::string failed_string{ "invalid control data: " };

    
    if (failed_control_data & FAILED_HEATERS)
    {
        failed_string += "heaters ";
    }
    if (failed_control_data & FAILED_COOLER)
    {
        failed_string += "cooler ";
    }
    if (failed_control_data & FAILED_CAMERA_TOGGLE)
    {
        failed_string += "camera_control ";
    }

    failed_string += '\n';

    return failed_string;
}

std::string failure_detection(const control_data& ctrl_data, const sensor_data& sensor_input)
{
    static control_data new_ctrl;
    static sensor_data new_sensor;

    if (sensor_data_is_empty(new_sensor) || control_data_is_empty(new_ctrl))
    {
        new_ctrl = ctrl_data;
        new_sensor = sensor_input;
        return {""};
    }

    
}
