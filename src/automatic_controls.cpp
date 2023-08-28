#include "automatic_controls.hpp"
#include <iostream>
#include <bitset>

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
    if (mean_temp(data) > MAX_TEMP) {
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
    if (mean_temp(data) > MAX_TEMP) {
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
    if(mean_temp(data) > COOLING_TEMP) {
        cooling = 1;
    } else {
        cooling = 0;
    }
    return cooling;
}
//Loop block for automation
void automatic_loop(const sensor_data &sens_data, control_data& ctrl_data, const json& control_json)
{
    if (!control_json["conveyor_manual_control"]) {
        ctrl_data.speed_of_conveyor = conveyor_control(sens_data);
    }
    if (!control_json["heater1_manual_control"]) {
        ctrl_data.heaters = heating_control(sens_data,HEATER_1);
    }
    if (!control_json["heater2_manual_control"]) {
        ctrl_data.heaters = heating_control(sens_data,HEATER_2);
    }
    if (!control_json["heater3_manual_control"]) {
        ctrl_data.heaters = heating_control(sens_data,HEATER_3);
    }
    if (!control_json["cooler_manual_control"]) {
        ctrl_data.cooler = cooling_control(sens_data);
    }
}
