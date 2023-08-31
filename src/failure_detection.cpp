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

bool heater_on(const control_data& ctrl_data, uint8_t index)
{
    if (ctrl_data.heaters & (1 << index))
        return true;
    return false;
}

int16_t mean_temperature(const sensor_data& sensor_input)
{
    return (sensor_input.temp_sensor01 +
        sensor_input.temp_sensor02 +
        sensor_input.temp_sensor03 +
        sensor_input.temp_sensor04 +
        sensor_input.temp_sensor05 +
        sensor_input.temp_sensor06 +
        sensor_input.temp_sensor07 +
        sensor_input.temp_sensor08 +
        sensor_input.temp_sensor09 +
        sensor_input.temp_sensor10) / 10;
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
    static control_data previous_ctrl;
    static sensor_data previous_sensor;
    static uint8_t rounds_of_temperature_anomalies{ 0 };
    static uint8_t rounds_of_conveyor_anomalies{ 0 };


    if (sensor_data_is_empty(previous_sensor) || control_data_is_empty(previous_ctrl))
    {
        previous_ctrl = ctrl_data;
        previous_sensor = sensor_input;
        return {""};
    }

    if (mean_temperature(sensor_input) < mean_temperature(previous_sensor) &&
        sensor_input.speed_of_conveyor >= previous_sensor.speed_of_conveyor &&
        heater_on(ctrl_data, 0) && heater_on(ctrl_data, 1)
        && heater_on(ctrl_data, 2) && ctrl_data.cooler <= previous_ctrl.cooler)
    {
        ++rounds_of_temperature_anomalies;
    }
    else
    {
        if(rounds_of_temperature_anomalies > 0)
        --rounds_of_temperature_anomalies;
    }

    if (sensor_input.speed_of_conveyor <= previous_sensor.speed_of_conveyor
        && ctrl_data.speed_of_conveyor > sensor_input.speed_of_conveyor)
    {
            ++rounds_of_conveyor_anomalies;
    }
    else
    {
        if(rounds_of_conveyor_anomalies > 0)
        --rounds_of_conveyor_anomalies;
    }

    std::string possible_failures{ "" };
    
    possible_failures += invalid_control_data(ctrl_data);
    possible_failures += failed_temp_sensors(sensor_input);

    if (rounds_of_conveyor_anomalies >= 5)
    {
        possible_failures += "conveyor possibly faulty\n";
    }
    if (rounds_of_temperature_anomalies >= 5)
    {
        possible_failures += "heaters and/or cooler possibly faulty\n";
    }

    previous_ctrl = ctrl_data;
    previous_sensor = sensor_input;

    return possible_failures;
}
