#ifndef AUTOMATIC_CONTROLS_HPP
#define AUTOMATIC_CONTROLS_HPP

#include "data_structures.hpp"
#include "../external/json.hpp"
#include <iostream>
#include <bitset>
#include <chrono>

using json = nlohmann::json;

constexpr int16_t MAX_TEMP = 800;
constexpr int16_t COOLING_TEMP = 400;
constexpr uint8_t OPTIMAL_SOC = 230;

//IN MAIN FILE
/*
    //FOR AUTOMATION
    auto start_time = std::chrono::system_clock::now();
    auto heater_timing = std::chrono::system_clock::now();
    bool heater_period = true;
    bool start_period = true;

    heater_period_timing(start_time, heater_timing, heater_period, start_period);
    automatic_loop(sensor_input,ctrl_data,control_data_json,heater_period,start_period);
*/

int16_t highest_temp(const sensor_data &data);

int16_t mean_temp(const sensor_data &data);

uint8_t conveyor_control(const sensor_data& data);

uint8_t heating_control(const sensor_data &data, uint8_t heater);

uint8_t cooling_control(const sensor_data &data);

void automatic_loop(const sensor_data &sens_data, control_data& ctrl_data, const json& control_json,const bool& heater_period,const bool& start_period);

void heater_period_timing(std::chrono::_V2::system_clock::time_point &start_time, std::chrono::_V2::system_clock::time_point &heater_timing, bool &heater_period, bool &start_period);

#endif
