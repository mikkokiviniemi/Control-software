#ifndef INPUT_VALIDATION_HPP
#define INPUT_VALIDATION_HPP

#include "data_structures.hpp"
#include <iostream>

//for input validation
constexpr uint16_t FAILED_TEMP_SENSOR01 { 0b1 };
constexpr uint16_t FAILED_TEMP_SENSOR02 { 0b10 };
constexpr uint16_t FAILED_TEMP_SENSOR03 { 0b100 };
constexpr uint16_t FAILED_TEMP_SENSOR04 { 0b1000 };
constexpr uint16_t FAILED_TEMP_SENSOR05 { 0b10000 };
constexpr uint16_t FAILED_TEMP_SENSOR06 { 0b100000 };
constexpr uint16_t FAILED_TEMP_SENSOR07 { 0b1000000 };
constexpr uint16_t FAILED_TEMP_SENSOR08 { 0b10000000 };
constexpr uint16_t FAILED_TEMP_SENSOR09 { 0b100000000 };
constexpr uint16_t FAILED_TEMP_SENSOR10 { 0b1000000000 };

constexpr uint8_t FAILED_HEATERS{ 0b1 };
constexpr uint8_t FAILED_COOLER{ 0b1 };
constexpr uint8_t FAILED_CAMERA_TOGGLE{ 0b1 };

constexpr uint8_t HEATER_MAX_VALUE{ 0b00000111 };

//function for data input validation
inline uint16_t temperature_input_validation(const sensor_data& sensor_input) {
    uint16_t failed_mask{ 0 };
    if (sensor_input.temp_sensor01 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR01;
        std::cout << "temp_sensor01 reading out of range: " << sensor_input.temp_sensor01 << '\n';
    }
    if (sensor_input.temp_sensor02 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR02;
        std::cout << "temp_sensor02 reading out of range: " << sensor_input.temp_sensor02 << '\n';
    }
    if (sensor_input.temp_sensor03 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR03;
        std::cout << "temp_sensor03 reading out of range: " << sensor_input.temp_sensor03 << '\n';
    }
    if (sensor_input.temp_sensor04 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR04;
        std::cout << "temp_sensor04 reading out of range: " << sensor_input.temp_sensor04 << '\n';
    }
    if (sensor_input.temp_sensor05 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR05;
        std::cout << "temp_sensor05 reading out of range: " << sensor_input.temp_sensor05 << '\n';
    }
    if (sensor_input.temp_sensor06 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR06;
        std::cout << "temp_sensor06 reading out of range: " << sensor_input.temp_sensor06 << '\n';
    }
    if (sensor_input.temp_sensor07 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR07;
        std::cout << "temp_sensor07 reading out of range: " << sensor_input.temp_sensor07 << '\n';
    }
    if (sensor_input.temp_sensor08 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR08;
        std::cout << "temp_sensor08 reading out of range: " << sensor_input.temp_sensor08 << '\n';
    }
    if (sensor_input.temp_sensor09 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR09;
        std::cout << "temp_sensor09 reading out of range: " << sensor_input.temp_sensor09 << '\n';
    }
    if (sensor_input.temp_sensor10 > 1100 || sensor_input.temp_sensor01 < -400)
    {
        failed_mask |= FAILED_TEMP_SENSOR10;
        std::cout << "temp_sensor10 reading out of range: " << sensor_input.temp_sensor10 << '\n';
    }
    return failed_mask;
}


//function for ctrl data input validation
inline uint8_t control_input_validation(const control_data& ctrl_data)
{
    uint8_t failed_mask{ 0 };

    if (ctrl_data.heaters > HEATER_MAX_VALUE)
    {
        failed_mask |= FAILED_HEATERS;
        std::cout << "Invalid control_data for heaters: "
            << static_cast<int16_t>(ctrl_data.heaters) << '\n';
    }
    if (ctrl_data.cooler > 1)
    {
        failed_mask |= FAILED_COOLER;
        std::cout << "Invalid control_data for cooler: "
            << static_cast<int16_t>(ctrl_data.cooler) << '\n';
    }
    if (ctrl_data.camera_toggle > 1)
    {
        std::cout << "Invalid control_data for camera_toggle: "
            << static_cast<int16_t>(ctrl_data.camera_toggle) << '\n';
        failed_mask |= FAILED_CAMERA_TOGGLE;
    }

    return failed_mask;
}

#endif
