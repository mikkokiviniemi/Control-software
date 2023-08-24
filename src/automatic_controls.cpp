#include "automatic_controls.hpp"

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
    if (mean_temp(data) > MAX_TEMP) {
        soc = 0;
    } else {
        soc = OPTIMAL_SOC;
    }
    return soc;
}
//Automatic control block for heating control. Takes values from sensor_data and makes changes to heating.
uint8_t heating_control(const sensor_data &data, uint8_t heater)
{
    uint8_t heaters = 0b0000;
    if (mean_temp(data) > MAX_TEMP) {
        heaters &= ~heater;
    }else {
        heaters |= heater;
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
void automatic_loop(const sensor_data &sens_data, control_data& ctrl_data)
{
    //if (!crtl_data.manual_convoyer) :
    ctrl_data.speed_of_conveyor = conveyor_control(sens_data);
    //if (!crtl_data.manual_heating_1):
    ctrl_data.heaters = heating_control(sens_data,HEATER_1);
    //if (!crtl_data.manual_heating_2):
    ctrl_data.heaters = heating_control(sens_data,HEATER_2);
    //if (!crtl_data.manual_heating_3):
    ctrl_data.heaters = heating_control(sens_data,HEATER_3);
    //if (!crtl_data.manual_cooling): 
    ctrl_data.cooler = cooling_control(sens_data);
}
