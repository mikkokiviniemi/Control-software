#ifndef FAILURE_DETECTION_HPP
#define FAILURE_DETECTION_HPP

#include "data_structures.hpp"

#include <string>

std::string failed_temp_sensors(const sensor_data& sensor_input);
std::string invalid_control_data(const control_data& ctrl_data);

bool control_data_is_empty(const control_data& ctrl_data);
bool sensor_data_is_empty(const sensor_data& sensor_input);

#endif
