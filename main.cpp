#include "data_structures.hpp"
#include "text_ui.hpp"
#include "dummy_data.hpp"

#include <iostream>
#include <vector>
#include <chrono>


    


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
        json input1 = {
            {"speed_of_conveyor", 5},
            {"heater_1", true},
            {"heater_2", false},
            {"heater_3", true},
            {"cooler", false},
            {"qc_camera_status", true},
            {"temp_sensor01", 25.5},
            {"temp_sensor02", 26.0},
            {"temp_sensor03", 25.8},
            {"temp_sensor04", 27.3},
            {"temp_sensor05", 24.9},
            {"temp_sensor06", 26.5},
            {"temp_sensor07", 25.7},
            {"temp_sensor08", 26.2},
            {"temp_sensor09", 25.1},
            {"temp_sensor10", 26.7},
            {"time_stamp", 1630000000}
        };
        json input2 = {
            {"qc_camera_fails", nullptr},
            {"time_stamp", 1630000000}
        };
        json output;
        json_ui(output,input1,input2);

    }

    return 0;
}
