#include "data_structures.hpp"
#include "text_ui.hpp"
#include "dummy_data.hpp"
#include "json_output.hpp"
#include "input_validation.hpp"
#include "automatic_controls.hpp"


#include "external/json.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

control_data ctrl_data{};
std::mutex mtx; 



int main()
{
    //FOR AUTOMATION

    auto start_time = std::chrono::system_clock::now();
    auto heater_timing = std::chrono::system_clock::now();
    bool heater_period = true;
    bool start_period = true;

    uint16_t failed_sensor_input_validation{ 0 };
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
            if (std::chrono::system_clock::now() - start_time > std::chrono::seconds(20)) {
                start_period = false;
                if (std::chrono::system_clock::now() - heater_timing > std::chrono::seconds(5) && heater_period) { 
                    heater_period = false;
                    heater_timing = std::chrono::system_clock::now();
                }
                if (std::chrono::system_clock::now() - heater_timing > std::chrono::seconds(10) && !heater_period) { 
                    heater_period = true;
                    heater_timing = std::chrono::system_clock::now();
                }
            }
            automatic_loop(sensor_input,ctrl_data,output,heater_period,start_period);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    std::thread data_thread([&]() {
        while (is_running) {
                dummy_data_generator(sensor_input, ctrl_data);
                std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
                sensor_input.time_stamp = { std::chrono::system_clock::to_time_t(now) };
                input1 = create_output_sensor_data(sensor_input, ctrl_data);
                input2 = create_camera_feed_output(sensor_input);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });

    automation_thread.join();
    data_thread.join();
    ui_thread.join();


    return 0;
}


