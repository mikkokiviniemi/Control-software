#include "text_ui.hpp"

#include <iostream>
#include <chrono>

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

struct control_data {
    uint8_t speed_of_conveyor;
    uint8_t heaters_cooler;
    uint8_t camera_toggle;
};

/*
JSON Output:
{
    "speed_of_conveyor" : int,
    "heater_1": bool,
    "heater_2": bool,
    "heater_3": bool,
    "cooler": bool,
    "qc_camera_status": bool,
    "temp_sensor01": float,
    "temp_sensor02": float,
    "temp_sensor03": float,
    "temp_sensor04": float,
    "temp_sensor05": float,
    "temp_sensor06": float,
    "temp_sensor07": float,
    "temp_sensor08": float,
    "temp_sensor09": float,
    "temp_sensor10": float,
    "time_stamp": value
}
*/
/*
JSON Output_2:

{
    "qc_camera_fails": ???,
    "time_stamp": value
}
*/

/*
JSON Commands:
{
    "speed_of_conveyor" : int,
    "heater_1": bool,
    "heater_2": bool,
    "heater_3": bool,
    "cooler": bool,
    "qc_camera_status": bool,
}
*/

int main()
{
    bool systems_online{ true };
    uint8_t speed_of_conveyor = 0x00;
    uint8_t heaters_cooler = 0x00;
    uint8_t camera_toggle = 0x00;

    /*bool manual_heaters{ false };
    bool manual_cooler{ false };
    bool manual_conveyor{ false };
    bool qc_camera{ false };
    */
    

    while (systems_online)
    {
        //read_sensor_data()
        
    //if UI decides to manual override, it will circumvent automatic control
        //read_ui_control()


    // The control functions writes the control data directly to shared memory
        //conveyor_control()
        //heating_control()
        //cooling_control()
        
        //data_transformation()
        //send_data_to_ui()
        //if (logging_enabled())
        {
            //log_data()    
        }


        ui_loop(systems_online, speed_of_conveyor, heaters_cooler, camera_toggle);

    }

    return 0;
}
