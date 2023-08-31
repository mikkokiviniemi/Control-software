#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H
#include "json.hpp"
#include "data_structures.hpp"

#include <string>

using json = nlohmann::json;

json create_output_sensor_data(const sensor_data& sensor_input, const control_data& ctrl_data, const std::string& failures);

control_data json_to_control_data(const json& json_elem);

int conveyor_speed_raw_to_ui(uint8_t conveyor_speed);
uint8_t conveyor_speed_ui_to_raw(int conveyor_speed);

float temperature_raw_to_ui(int16_t temperature);

#endif
