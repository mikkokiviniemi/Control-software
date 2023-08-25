#include "data_structures.hpp"
#include "text_ui.hpp"
#include "dummy_data.hpp"
#include "json_output.hpp"
#include "input_validation.hpp"
#include "automatic_controls.hpp"
#include "mqtt_client.hpp"


#include "external/json.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

control_data ctrl_data{};
std::mutex mtx; 


int main()
{

/*     uint16_t failed_sensor_input_validation{ 0 };
    uint8_t failed_control_input_validation{ 0 };

    sensor_data sensor_input{ 0, 0, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0};

    json input1;
    json input2;
    json output
    {
    {"speed_of_conveyor", 0 },
    {"heater_1", false },
    {"heater_2", false },
    {"heater_3", false },
    {"cooler", false },
    {"qc_camera_status", false },
    {"conveyor_manual_control", false},
    {"heater1_manual_control", false},
    {"heater2_manual_control" , false},
    {"heater3_manual_control", false},
    {"cooler_manual_control", false}

    };
    dummy_data_generator(sensor_input, ctrl_data);
    bool is_running = true;

    std::thread ui_thread([&]() {
        while (is_running) {
            json_ui(output, input1, input2);
            std::lock_guard<std::mutex> lock(mtx);
            ctrl_data = json_to_control_data(output);
        }
    });


    std::thread automation_thread([&]() {
        while (is_running) {
            automatic_loop(sensor_input,ctrl_data,output);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    std::thread data_thread([&]() {
        while (is_running) {
            {
                dummy_data_generator(sensor_input, ctrl_data);
                std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
                sensor_input.time_stamp = { std::chrono::system_clock::to_time_t(now) };
            }
            {
                input1 = create_output_sensor_data(sensor_input, ctrl_data);
                input2 = create_camera_feed_output(sensor_input);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });

    automation_thread.join();
    data_thread.join();
    ui_thread.join(); */

    MQTT_Client test (ADDRESS, USER_ID);
    test.connect_broker();
    test.subscribe(TOPIC_RECEIVE);
    while (true) {
        std::cout << "p to publish, q to quit:\n";
        std::string option;
        std::getline(std::cin, option);
        if ( option == "p") { 
            test.publish(TOPIC_SEND, PAYLOAD);
        }
        if (option == "q") {
            break;
        }

    }


    return 0;
}


