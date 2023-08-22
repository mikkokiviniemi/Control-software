#include "json_output.hpp"
#include "json.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>




json create_output_sensor_data(const sensor_data& test) {
    std::stringstream time;
    time << std::put_time(std::localtime(&test.time_stamp), "%FT%TGMT+2");
    json output_data = {
    {"speed_of_conveyor", test.speed_of_conveyor}, // uint16_t 
    {"heater_1", false},
    {"heater_2", false},
    {"heater_3", false},
    {"cooler", false},
    {"qc_camera_status", false},
    {"temp_sensors", {
        static_cast<float>(test.temp_sensor01 / 10), 
        static_cast<float>(test.temp_sensor02 / 10),
        static_cast<float>(test.temp_sensor03 / 10),
        static_cast<float>(test.temp_sensor04 / 10),
        static_cast<float>(test.temp_sensor05 / 10),
        static_cast<float>(test.temp_sensor06 / 10),
        static_cast<float>(test.temp_sensor07 / 10),
        static_cast<float>(test.temp_sensor08 / 10),
        static_cast<float>(test.temp_sensor09 / 10),
        static_cast<float>(test.temp_sensor10 / 10)
        }},
    {"time_stamp", time.str()}
    };
    return output_data;
}

json create_camera_feed_output(const sensor_data& test) {
    json camera_feed_output = {
    {"qc_camera_fails", test.qc_camera_fails},
    {"time_stamp", test.time_stamp}
    };
    return camera_feed_output;
}
