#include "data_structures.hpp"
#include "text_ui.hpp"
#include "dummy_data.hpp"
#include "json_output.hpp"
#include "automatic_controls.hpp"

#include "external/json.hpp"

#include <iostream>
#include <chrono>


int main()
{
  
    sensor_data sensor_input{ 0, 0, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};
    control_data ctrl_data{};

    json input1;
    json input2;
    json output
    {
    {"speed_of_conveyor", 0 },
    {"heater_1", false },
    {"heater_2", false },
    {"heater_3", false },
    {"cooler", false },
    {"qc_camera_status", false }
    };

    while (true)
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
        sensor_input = dummy_data_generator(sensor_input, ctrl_data);
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        
        sensor_input.time_stamp = { std::chrono::system_clock::to_time_t(now) };

        input1 = create_output_sensor_data(sensor_input, ctrl_data);
        input2 = create_camera_feed_output(sensor_input);

        json_ui(output, input1, input2);

        ctrl_data = json_to_control_data(output);      
        automatic_loop(sensor_input,ctrl_data);


    }

    return 0;
}
