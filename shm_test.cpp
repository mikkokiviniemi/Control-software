
#include "shared_memory_wrapper.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main()
{
    std::string filename{ "shm_test" };
    simulation_shm_wrapper shm_wrapper{filename};

    int camera_status{ 0 };

    while (camera_status != 1)
    {
        std::cout << "Temp_sensors:\n";
        for (uint8_t i{ 0 }; i < 10; ++i)
        {
            std::cout << shm_wrapper.read_temperature_sensor(i) << '\n';
        }
        std::cout << "Conveyor_speed: " << static_cast<int>( shm_wrapper.read_conveyor_speed_sensor() ) << '\n';
        std::cout << "Camera_feed: " << shm_wrapper.read_qc_camera_feed() << '\n';

        std::cout << "Please give input (1 to quit, otherwise loop over): ";
        int input;
        std::cin >> input;

        shm_wrapper.set_camera_status(static_cast<uint8_t>(input));
        camera_status = input;
    }

    std::cout << "Quit second app!\n";


    return 0;
}
