#include "automatic_controls.hpp"

//Calculates highest temperature from sensor data
int16_t highest_temp(const sensor_data& data) {
    int16_t highest_temp = data.temp_sensor01;
    int16_t temp_sensors[] = {
        data.temp_sensor02, data.temp_sensor03, data.temp_sensor04, data.temp_sensor05,
        data.temp_sensor06, data.temp_sensor07, data.temp_sensor08, data.temp_sensor09,
        data.temp_sensor10
    };
    //Compare temps to highest_temp
    for (const int16_t& temp : temp_sensors) {
        if (temp > highest_temp) {
            highest_temp = temp;
        }
    }
    return highest_temp;
}

//Calculates mean temperature from sensor data
int16_t mean_temp (const sensor_data& data) {
    int16_t addition = 
    data.temp_sensor01 +
    data.temp_sensor02 +
    data.temp_sensor03 +
    data.temp_sensor04 +
    data.temp_sensor05 +
    data.temp_sensor06 +
    data.temp_sensor07 +
    data.temp_sensor08 +
    data.temp_sensor09 +
    data.temp_sensor10;
    return addition/10;
}

//Automatic control block for conveyor speed control. Takes values from sensor_data and makes changes to conveyor speed.
uint8_t conveyor_control(const sensor_data &data)
{
    uint8_t soc;
    if (highest_temp(data) > MAX_TEMP) {
        soc = 0;
    } else {
        soc = OPTIMAL_SOC;
    }
    return soc;
}

//Toggle heater
void toggle_heater(uint8_t& heaters, const uint8_t heater, bool state) {
    if (state) {
        heaters |= heater; // Set the bit to 1
    } else {
        heaters &= ~heater; // Set the bit to 0
    }
}

//Automatic control block for heating control. Takes values from sensor_data and makes changes to heating.
uint8_t heating_control(const sensor_data &data, uint8_t heater)
{
    uint8_t heaters;
    if (highest_temp(data) > MAX_TEMP) {
        toggle_heater(heaters,heater,false);
    }else {
        toggle_heater(heaters,heater,true);
    }
    return heaters;
}

//Automatic control block for cooling control. Takes values from sensor_data and makes changes to cooling.
uint8_t cooling_control(const sensor_data &data)
{
    uint8_t cooling;
    if(highest_temp(data) > COOLING_TEMP) {
        cooling = 1;
    } else {
        cooling = 0;
    }
    return cooling;
}

//Loop block for automation
void automatic_loop(const sensor_data &sens_data, control_data& ctrl_data, const json& control_json, const bool& heater_period, const bool& start_period)
{
    bool failure_mode = false; //IF no camera fails -> failure_mode = false
    
    if (!control_json["conveyor_manual_control"]) { // Check if manual control is on
        if (failure_mode || start_period) { //If failure mode or start_period on
            ctrl_data.speed_of_conveyor = 0;
        } else {
            ctrl_data.speed_of_conveyor = conveyor_control(sens_data);
        }
    }
    if (!control_json["heater_1_manual_control"]) { // Check if manual control is on
        if (failure_mode || !heater_period) { //If failure mode on or not heater_period
            toggle_heater(ctrl_data.heaters,HEATER_1,false);
        } else {
            ctrl_data.heaters = heating_control(sens_data,HEATER_1);
        }
    }
    if (!control_json["heater_2_manual_control"]) { // Check if manual control is on
        if (failure_mode || !heater_period) { //If failure mode on or not heater_period
            toggle_heater(ctrl_data.heaters,HEATER_2,false);
        } else {
            ctrl_data.heaters = heating_control(sens_data,HEATER_2);
        }
    }
    if (!control_json["heater_3_manual_control"]) { // Check if manual control is on
        if (failure_mode || !heater_period) { //If failure mode on or not heater_period
            toggle_heater(ctrl_data.heaters,HEATER_3,false);
        } else {
            ctrl_data.heaters = heating_control(sens_data,HEATER_3);
        }
    }
    if (!control_json["cooler_manual_control"]) { // Check if manual control is on
        if (failure_mode) { //If failure mode on
            ctrl_data.cooler = 0;
        } else {
            ctrl_data.cooler = cooling_control(sens_data);
        }
    }
}

//Changes heater_period boolean according to timing
void heater_period_timing(std::chrono::_V2::system_clock::time_point& start_time, 
                   std::chrono::_V2::system_clock::time_point& heater_timing, 
                   bool& heater_period, bool& start_period) {
    if (std::chrono::system_clock::now() - start_time > STARTING_PERIOD_TIME) {
        start_period = false;
        if (std::chrono::system_clock::now() - heater_timing > HEATERS_ON_PERIOD && heater_period) { 
            heater_period = false;
            heater_timing = std::chrono::system_clock::now();
        }
        if (std::chrono::system_clock::now() - heater_timing > HEATERS_OFF_PERIOD && !heater_period) { 
            heater_period = true;
            heater_timing = std::chrono::system_clock::now();
        }
    }
}