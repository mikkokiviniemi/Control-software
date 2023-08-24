
#include "shared_memory_wrapper.hpp"

#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

int main()
{
    std::string filename{ "shm_test" };
    simulation_shm_wrapper shm_wrapper{filename};

    for (int16_t index{ 0 }; index < 10; ++index)
    {
        *(reinterpret_cast<int16_t*>(shm_wrapper.value_ptr + index * 2)) = index;
    }
    *(shm_wrapper.value_ptr + 20) = 150;
    *(reinterpret_cast<uint16_t*>(shm_wrapper.value_ptr + 25)) = 0b0101010101010101;

    int camera_status{ 0 };

    int input{0};

    // while (input == 0)
    // {
    //     std::cout << "Give value to input (0 to quit): ";
    //     std::cin >> input;
    // }

    while (camera_status != 1)
    {
        std::cout << "Waiting...\n";
        std::this_thread::sleep_for(2000ms);
        uint8_t input{ *(reinterpret_cast<uint8_t*>(shm_wrapper.value_ptr + 24)) };
        camera_status = static_cast<int>(input);
    }

    std::cout << "Quit first app!\n";


    return 0;
}
