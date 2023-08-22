#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H
#include "json.hpp"

#include <array>

using json = nlohmann::json;

json create_output_sensor_data(const sensor_data& test);
json create_camera_feed_output(const sensor_data& test);

// time functions for time_stamp
// std::put_time(std::localtime(/*std::time_t*/), "%FT%TGMT+2")
// std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
// std::time_t time_var{ std::chrono::system_clock::to_time_t(now) };
#endif