#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H
#include "json.hpp"
// #include "json_fwd.hpp"
#include <array>

using json = nlohmann::json;

// test purposes
struct sensor_data {
    uint8_t speed_of_conveyor;
    uint16_t qc_camera_fails;

    int16_t temp_sensor01;
    int16_t temp_sensor02;
    int16_t temp_sensor03;
    int16_t temp_sensor04;
    int16_t temp_sensor05;
    int16_t temp_sensor06;
    int16_t temp_sensor07;
    int16_t temp_sensor08;
    int16_t temp_sensor09;
    int16_t temp_sensor10;

    std::time_t time_stamp;
};


json create_output_sensor_data(const sensor_data& test);
json create_camera_feed_output(const sensor_data& test);

// time functions for time_stamp
// std::put_time(std::localtime(/*std::time_t*/), "%FT%TGMT+2")
// std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
// std::time_t time_var{ std::chrono::system_clock::to_time_t(now) };
#endif