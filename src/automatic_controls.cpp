#include "automatic_controls.hpp"
#include <iostream>
#include <bitset>
#include <chrono>
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
// max_temp (use for checking)

//vikatila

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
void automatic_loop(const sensor_data &sens_data, control_data& ctrl_data, const json& control_json)
{
    
    bool failure_mode = (sens_data.qc_camera_fails != 0); //IF no camera fails -> failure_mode = false
    
    if (!control_json["conveyor_manual_control"]) {
        if (failure_mode) {
            ctrl_data.speed_of_conveyor = 0;
        } else {
            ctrl_data.speed_of_conveyor = conveyor_control(sens_data);
        }
    }
    if (!control_json["heater1_manual_control"]) {
        if (failure_mode) {
            toggle_heater(ctrl_data.heaters,HEATER_1,false);
        } else {
            ctrl_data.heaters = heating_control(sens_data,HEATER_1);
        }
    }
    if (!control_json["heater2_manual_control"]) {
        if (failure_mode) {
            toggle_heater(ctrl_data.heaters,HEATER_2,false);
        } else {
            ctrl_data.heaters = heating_control(sens_data,HEATER_2);
        }
    }
    if (!control_json["heater3_manual_control"]) {
        if (failure_mode) {
            toggle_heater(ctrl_data.heaters,HEATER_3,false);
        } else {
            ctrl_data.heaters = heating_control(sens_data,HEATER_3);
        }
    }
    if (!control_json["cooler_manual_control"]) {
        if (failure_mode) {
            ctrl_data.cooler = 0;
        } else {
            ctrl_data.cooler = cooling_control(sens_data);
        }
    }
}
