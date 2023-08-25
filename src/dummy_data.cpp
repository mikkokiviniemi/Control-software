#include "dummy_data.hpp"

constexpr float CONVEYOR_HEAT_CONSTANT{ 3.6 };
constexpr float CONVEYOR_MAX_EFFICIENCY{ 0.44 };
constexpr int16_t HEATER_ON { 10 };
constexpr int16_t HEATER_OFF{ -5 };
constexpr int16_t COOLER_ON{ -10 };
constexpr uint16_t QC_CAMERA_FAILS{ 0b1010110101010101 };

//A simple function to modify each
void modify_temperature(sensor_data& dummy_data, int16_t modifier)
{
    dummy_data.temp_sensor01 += modifier;
    dummy_data.temp_sensor02 += modifier;
    dummy_data.temp_sensor03 += modifier;
    dummy_data.temp_sensor04 += modifier;
    dummy_data.temp_sensor05 += modifier;
    dummy_data.temp_sensor06 += modifier;
    dummy_data.temp_sensor07 += modifier;
    dummy_data.temp_sensor08 += modifier;
    dummy_data.temp_sensor09 += modifier;
    dummy_data.temp_sensor10 += modifier;
}

//A function to simulate the conveyors behaviour, and it's affects on temperature and speed
void simulate_conveyor(sensor_data& dummy_data, const control_data& ctrl_data)
{
    // Check if control_data speed is set higher than sensor speed, raise sensor speed
    if (ctrl_data.speed_of_conveyor > dummy_data.speed_of_conveyor)
    {
        ++dummy_data.speed_of_conveyor;
    }
    // Check if control_data speed is lower than sensor speed, lower sensor speed
    else if (ctrl_data.speed_of_conveyor < dummy_data.speed_of_conveyor)
    {
        --dummy_data.speed_of_conveyor;
    }
    
    // Check if speed is under 10% (ie. 25) then calculate the efficiency (linear from 44% to 0%)
    // If speed is over 10% (ie. 25 or over) efficiency is constant with speed 44%
    if (dummy_data.speed_of_conveyor < 25)
    {
        float temperature_modifier{ (1.0f - (CONVEYOR_MAX_EFFICIENCY * static_cast<float>(dummy_data.speed_of_conveyor) / 25.0f))
        * CONVEYOR_HEAT_CONSTANT * static_cast<float>(dummy_data.speed_of_conveyor) };
        modify_temperature(dummy_data, static_cast<int16_t>(temperature_modifier));
    }
    else
    {
        float temperature_modifier{ (1.0f - CONVEYOR_MAX_EFFICIENCY) * CONVEYOR_HEAT_CONSTANT * static_cast<float>(dummy_data.speed_of_conveyor) };
        modify_temperature(dummy_data, static_cast<int16_t>(temperature_modifier));
    }

}

//Simple data_generator
 void dummy_data_generator(sensor_data& dummy_data, const control_data& ctrl_data) {
    if (ctrl_data.heaters & HEATER_1)
    {
        modify_temperature(dummy_data, HEATER_ON);
    }
    else
    {
        modify_temperature(dummy_data, HEATER_OFF);
    }
    if (ctrl_data.heaters & HEATER_2)
    {
        modify_temperature(dummy_data, HEATER_ON);
    }
    else
    {
        modify_temperature(dummy_data, HEATER_OFF);
    }
    if (ctrl_data.heaters & HEATER_3)
    {
        modify_temperature(dummy_data, HEATER_ON);
    }
    else
    {
        modify_temperature(dummy_data, HEATER_OFF);
    }
    if (ctrl_data.cooler == 1)
    {
        modify_temperature(dummy_data, COOLER_ON);
    }

    simulate_conveyor(dummy_data, ctrl_data);

    dummy_data.qc_camera_fails = QC_CAMERA_FAILS;

}