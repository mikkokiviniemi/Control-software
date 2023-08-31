#include "data_structures.hpp"
#include "text_ui.hpp"
#include "json_output.hpp"
#include "input_validation.hpp"
#include "automatic_controls.hpp"

#include "mqtt_client.hpp"
#include "shared_memory_wrapper.hpp"


#include "json_fwd.hpp"
#include "json.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

int stop{ 0 };

void stop_function()
{
    std::cout << "Input 1 to quit:\n";
    std::cin >> stop;

    if (stop == 1)
    {
        return;
    }
}

int main()
{
    //FOR AUTOMATION
    auto start_time = std::chrono::system_clock::now();
    auto heater_timing = std::chrono::system_clock::now();
    bool heater_period = true;
    bool start_period = true;

    //simulation_shm_wrapper sensor_data_input{ std::string{"shm_file"} };
    simulation_shm_wrapper shm{ std::string{"Sim/simulation_shm"} };
    
    control_data ctrl_data{0,0,0,0};
    sensor_data sensor_input{ 0, 0, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0 };
        
    json sensor_data_json; // output to UI
    json control_data_json // input from UI
    {
    {"speed_of_conveyor", 0 },
    {"heater_1", false },
    {"heater_2", false },
    {"heater_3", false },
    {"cooler", false },
    {"qc_camera_toggle", false },
    {"conveyor_manual_control", true},
    {"heater_1_manual_control", true},
    {"heater_2_manual_control", true},
    {"heater_3_manual_control", true},
    {"cooler_manual_control", true}
    };

    // // Initialize and connect MQTT client, subscribe to topic
    MQTT_Client mqtt_client (ADDRESS, USER_ID_CONTROL);
    mqtt_client.connect_broker();
    mqtt_client.subscribe(TOPIC_RECEIVE);

    // set timer
    std::chrono::time_point<std::chrono::system_clock> mqtt_timer = std::chrono::system_clock::now();

    shm.set_control_data(ctrl_data);

    std::jthread stop_thread(stop_function);

    while (stop != 1)
    {
        try
        {
            
        // Read sensor inputs from shared memory
        shm.read_sensor_inputs(sensor_input);

        // Add timestamp to sensor input data
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        sensor_input.time_stamp = std::chrono::system_clock::to_time_t(now);

        // Send/receive rate to/from UI is set to one second
        if (now - mqtt_timer >= std::chrono::milliseconds(1000))
        {
            // Create output json to UI
            sensor_data_json = create_output_sensor_data(sensor_input, ctrl_data);

            // Publish data to UI
            mqtt_client.publish(TOPIC_SEND_SENSOR, sensor_data_json.dump());
            
            // std::cout << "Sensor_data published:\n" << sensor_data_json << '\n';
            
            // Fetch MQTT control-data and store it in a json
            control_data_json = mqtt_client.input_control_data;
            ctrl_data = json_to_control_data(control_data_json);
            mqtt_timer = now;
        }
        
        // Automatic loop changes the control data
        heater_period_timing(start_time, heater_timing, heater_period, start_period);
        automatic_loop(sensor_input,ctrl_data,control_data_json,heater_period,start_period);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Send control data to simulation
        shm.set_control_data(ctrl_data);
        }
        catch(const json::exception& e)
        {
            std::cerr << "JSON error: " << e.what() << '\n';
        }    
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
