#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <cstdint>
#include <ctime>

constexpr uint8_t HEATER_1  { 0b1 };
constexpr uint8_t HEATER_2  { 0b10 };
constexpr uint8_t HEATER_3  { 0b100 };

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
