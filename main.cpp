#include "data_structures.hpp"
#include "text_ui.hpp"
#include "dummy_data.hpp"
#include "json_output.hpp"
#include "automatic_controls.hpp"

#include "external/json.hpp"

#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

std::mutex mtx; 


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
    sensor_input = dummy_data_generator(sensor_input, ctrl_data);
    bool is_running = true;
    std::thread ui_thread([&]() {
        while (is_running) {
            json_ui(output, input1, input2);
            ctrl_data = json_to_control_data(output);
        }
    });

    std::thread automation_thread([&]() {
        while (is_running) {
            std::lock_guard<std::mutex> lock(mtx);
            automatic_loop(sensor_input,ctrl_data);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    std::thread data_thread([&]() {
        while (is_running) {
            {
                sensor_input = dummy_data_generator(sensor_input, ctrl_data);
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
    ui_thread.join();

    return 0;
}
