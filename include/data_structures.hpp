#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <cstdint>
#include <ctime>

constexpr uint8_t HEATER_1  { 0b1 };
constexpr uint8_t HEATER_2  { 0b10 };
constexpr uint8_t HEATER_3  { 0b100 };


/***************SHARED MEMORY SCHEMA***************************
*  SHM_ptr       = temperature 1         (2 bytes)   (input)  *
*  SHM_ptr + 2   = temperature 2         (2 bytes)   (input)  *
*  SHM_ptr + 4   = temperature 3         (2 bytes)   (input)  *
*  SHM_ptr + 6   = temperature 4         (2 bytes)   (input)  *
*  SHM_ptr + 8   = temperature 5         (2 bytes)   (input)  *
*  SHM_ptr + 10  = temperature 6         (2 bytes)   (input)  *
*  SHM_ptr + 12  = temperature 7         (2 bytes)   (input)  *
*  SHM_ptr + 14  = temperature 8         (2 bytes)   (input)  *
*  SHM_ptr + 16  = temperature 9         (2 bytes)   (input)  *
*  SHM_ptr + 18  = temperature 10        (2 bytes)   (input)  *
*  SHM_ptr + 20  = conveyor speed sensor (1 byte)    (input)  *
*  SHM_ptr + 21  = conveyor speed target (1 byte)    (output) *
*  SHM_ptr + 22  = heaters control       (1 byte)    (output) *
*  SHM_ptr + 23  = cooler control        (1 byte)    (output) *
*  SHM_ptr + 24  = camera control        (1 byte)    (output) *
*  SHM_ptr + 25  = camera data           (2 bytes)   (input)  *
**************************************************************/


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
