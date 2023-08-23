#ifndef AUTOMATIC_CONTROLS_HPP
#define AUTOMATIC_CONTROLS_HPP

#include "data_structures.hpp"
#include <iostream>

//Saadaan datat

//Lähetetään controllit

constexpr int16_t MAX_TEMP = 800;
constexpr int16_t COOLING_TEMP = 600;
constexpr uint8_t OPTIMAL_SOC = 255;

int16_t mean_temp(const sensor_data &data);

// conveyor_control()
uint8_t conveyor_control(const sensor_data& data);

//heating_control()
uint8_t heating_control(const sensor_data& data);

//cooling_control()
uint8_t cooling_control(const sensor_data& data);

void automatic_loop(const sensor_data &sens_data, control_data &ctrl_data);

#endif