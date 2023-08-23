#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <cstdint>
#include <ctime>

constexpr uint8_t HEATER_1  { 0b1 };
constexpr uint8_t HEATER_2  { 0b10 };
constexpr uint8_t HEATER_3  { 0b100 };

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


//Control data structure
struct control_data {
    uint8_t speed_of_conveyor;
    uint8_t heaters;
    uint8_t cooler;
    uint8_t camera_toggle;
};

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

#endif
