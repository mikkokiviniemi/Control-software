#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "data_structures.hpp"
#include "dummy_data.hpp"
#include "input_validation.hpp"
#include "automatic_controls.hpp"

TEST_CASE("Dummy data")
{

    sensor_data sensor_input{ 0, 0, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};
    control_data ctrl_data{ 150, 0b111, 1, 1 };

    dummy_data_generator(sensor_input, ctrl_data);

    CHECK(sensor_input.speed_of_conveyor == 1);   
    CHECK(sensor_input.temp_sensor01 == sensor_input.temp_sensor10);
    CHECK(sensor_input.temp_sensor01 == 273);
}

TEST_CASE("Data validation")
{

    sensor_data sensor_input{ 0, 0, 1250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};
    control_data ctrl_data{150, 0b111, 1, 1 };
    
    uint16_t failed_sensor_input_validation{ temperature_input_validation(sensor_input) };
    uint8_t failed_control_input_validation{ control_input_validation(ctrl_data) };

    CHECK((failed_sensor_input_validation & FAILED_TEMP_SENSOR01));
    CHECK(failed_control_input_validation == 0);

}

TEST_CASE("Control software")
{
     
}

TEST_CASE("Automatic control")
{
    sensor_data sensor_input{ 0, 0, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};
    CHECK(mean_temp(sensor_input) == 25);
    CHECK(conveyor_control(sensor_input) == OPTIMAL_SOC);
    CHECK(heating_control(sensor_input) == 0b00000111);
    CHECK(cooling_control(sensor_input) == 0);

}
