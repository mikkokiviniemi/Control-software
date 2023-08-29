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

int main()
{

 //   simulation_shm_wrapper sensor_data_input{ std::string{"shm_file"} };
    simulation_shm_wrapper shm{ std::string{"simulation_shm"} };
    
    
    uint16_t failed_sensor_input_validation{ 0 };
    uint8_t failed_control_input_validation{ 0 };

    std::mutex mtx; 

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
    {"qc_camera_status", false },
    {"conveyor_manual_control", true},
    {"heater1_manual_control", true},
    {"heater2_manual_control", true},
    {"heater3_manual_control", true},
    {"cooler_manual_control", true}
    };
    
    //control_data_json = UI_MQTT_PALIKKA(sensor_data_json, control_data_json, camera_feed_json);
    //

    //control_data_json = mqtt_vastaanottaja_palikka();
    // mqtt_lähettäjä_palikka(sensor_data_json);
    // mqtt_lähettäjä_palikka(camera_feed_json);

    // std::thread ui_thread([&]() {
    //     while (is_running) {
    //         // UI muuttaa output_jsonia - Tämä korvataan MQTT feedillä
    //         // MQTT feed, subscribe 1 topic (UI:lta control dataa), publish 2 topic (sensor_data ja camera_feed)
    //         json_ui(control_data_json, sensor_data_json, camera_feed_json);

            
    //         //Output json muuttaa control_dataa
    //         std::lock_guard<std::mutex> lock(mtx);
    //         ctrl_data = json_to_control_data(control_data_json);
    //     }
    // });
    
    /*
    Saadaan sensor data (YKKÖS RYHMÄLTÄ)
    Lähetetään sensor data ja control data (KOLMOS RYHMÄLLE)
    Saadaan json data (KOLMOS RYHMÄLTÄ)
    Tehdään automaatio
    lähetetään control data (YKKÖS RYHMÄLLE)

    */

    // // Initialize and connect MQTT client, subscribe to topic
    MQTT_Client mqtt_client (ADDRESS, USER_ID_CONTROL);
    mqtt_client.connect_broker();
    mqtt_client.subscribe(TOPIC_RECEIVE);

    // set timer
    std::chrono::time_point<std::chrono::system_clock> mqtt_timer = std::chrono::system_clock::now();

    shm.set_control_data(ctrl_data);

    int stop{ 0 };

    while (true)
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
            
            std::cout << "Sensor_data published:\n" << sensor_data_json << '\n';
            
            // Fetch MQTT control-data and store it in a json
            control_data_json = mqtt_client.input_control_data;
            ctrl_data = json_to_control_data(control_data_json);
            mqtt_timer = now;
        }
        
        // Automatic loop changes the control data
        automatic_loop(sensor_input, ctrl_data, control_data_json);

        // Send control data to simulation
        shm.set_control_data(ctrl_data);

        // std::cout << "Input 1 to exit, 0 to continue: ";
        // std::cin >> stop;

        // std::this_thread::sleep_for(std::chrono::milliseconds(500));

        

    }


    
    // std::thread automation_thread([&]() {
    //     while (is_running) {
    //         //Fetch MQTT control-data
           
    //         //Automatic loop muuttaa control_dataa
    //         automatic_loop(sensor_input,ctrl_data, control_data_json);
    //         std::this_thread::sleep_for(std::chrono::milliseconds(500));
    //     }
    // });
    // std::thread data_thread([&]() {
    //     while (is_running) {
    //             //Tuottaa dummy_dataa (muuttaa sensor_inputtia) ja laittaa timestampin
    //             dummy_data_generator(sensor_input, ctrl_data);
    //             std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    //             sensor_input.time_stamp = std::chrono::system_clock::to_time_t(now);

    //             //Tehdään (output jsonit jotka lähetetään UI:lle)
    //             sensor_data_json = create_output_sensor_data(sensor_input, ctrl_data);
    //             camera_feed_json = create_camera_feed_output(sensor_input);
    //             std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    //     }
    // });

    // automation_thread.join();
    // data_thread.join();
    // ui_thread.join(); 


    return 0;
}
