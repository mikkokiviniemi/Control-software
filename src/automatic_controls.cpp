#include "automatic_controls.hpp"

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

uint8_t conveyor_control(const sensor_data &data)
{
    uint8_t soc;
    if (mean_temp(data) > MAX_TEMP) {
        soc = 0;
    }else {
        soc = OPTIMAL_SOC;
    }
    return soc;
}

uint8_t heating_control(const sensor_data &data)
{
    uint8_t heaters;
    if (mean_temp(data) > MAX_TEMP) {
        heaters = 0;
    }else {
        heaters = HEATER_1 | HEATER_2 | HEATER_3;
    }
    return heaters;
}

uint8_t cooling_control(const sensor_data &data)
{
    if(mean_temp(data) > COOLING_TEMP) {
        return 1;
    }
    
    return 0;
}

void automatic_loop(const sensor_data &sens_data, control_data& ctrl_data)
{
    ctrl_data.speed_of_conveyor = conveyor_control(sens_data);
    ctrl_data.heaters = heating_control(sens_data);
    ctrl_data.cooler = cooling_control(sens_data);
}
