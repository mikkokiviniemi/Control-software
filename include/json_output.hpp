#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H
#include "json.hpp"
#include "data_structures.hpp"


using json = nlohmann::json;

json create_output_sensor_data(const sensor_data& sensor_input, const control_data& ctrl_data);
//json create_camera_feed_output(const sensor_data& sensor_input);

control_data json_to_control_data(const json& json_elem);

int conveyor_speed_raw_to_ui(uint8_t conveyor_speed);
uint8_t conveyor_speed_ui_to_raw(int conveyor_speed);


// time functions for time_stamp
// std::put_time(std::localtime(/*std::time_t*/), "%FT%TGMT+2")
// std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
// std::time_t time_var{ std::chrono::system_clock::to_time_t(now) };
#endif
