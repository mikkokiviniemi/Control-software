#include "dummy_data.hpp"

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;


dummy_shm_wrapper::dummy_shm_wrapper(const std::string& new_filename, std::size_t new_block_size)
    : filename{ new_filename }, block_size{ new_block_size }
{
    //Ensure file exists
    fs::path file_path{ filename };
    if (!fs::exists(file_path))
    {
        //If not, create it
        std::ofstream create_file(file_path);
    }

    //Attach raw void* to class
    ptr_to_memory = attach_memory_block();
    //Cast to uint8_t* to be used with the reading/writing functions
    value_ptr = reinterpret_cast<uint8_t*>(ptr_to_memory);
}

// Read temperature sensor ranging from 1 to 10, return -999 if wrong indexing
void dummy_shm_wrapper::set_temperature_sensor(int16_t value, uint8_t sensor_number)
{
    if ( sensor_number < 1 || sensor_number > 10 )
    {
        std::cerr << "Faulty index, returning -999\n";
        return;
    }
    *(reinterpret_cast<int16_t*>(value_ptr + (sensor_number - 1) * 2)) = value;
}

void dummy_shm_wrapper::set_conveyor_speed_sensor(uint8_t value)
{
    *(value_ptr + 20) = value;
}

void dummy_shm_wrapper::set_qc_camera_feed(uint16_t value)
{
    *(reinterpret_cast<uint16_t*>(value_ptr + 25)) = value;
}

uint8_t dummy_shm_wrapper::read_conveyor_target_speed() const
{
    return *(value_ptr + 21);
}

uint8_t dummy_shm_wrapper::read_heaters() const
{
    return *(value_ptr + 22);
}

uint8_t dummy_shm_wrapper::read_cooler() const
{
    return *(value_ptr + 23);
}

uint8_t dummy_shm_wrapper::read_camera_status() const
{
    return *(value_ptr + 24);
}

bool dummy_shm_wrapper::quit() const
{
    return (*(value_ptr + 27) == 1);
}


dummy_shm_wrapper::~dummy_shm_wrapper()
{
  
    if (destroy_memory_block())
    {
        std::cout << "Destroyed memory block\n";
    }
    else
    {
        std::cout << "Couldn't destroy memory block, could be already destroyed\n";
    }

    if (detach_memory_block())
    {
        std::cout << filename << " memory block detached succesfully\n";
    }
    else
        std::cout << filename << " memory block detachment unsuccesful\n";
}

// Function to get block-id: a numeric value associated with the file used for shared memory
int dummy_shm_wrapper::get_shared_block(std::size_t size)
{
    key_t key;

    // Request a key that is linked to a filename, so that other programs can access it
    key = ftok(filename.c_str(), 0);
    if (key == SHM_ERROR)
    {
        return SHM_ERROR;
    }

    // Get shared block-id (0666 read-write permissions for all). Create it if it doesn't exist
   return (shmget(key, size, 0666 | IPC_CREAT ) );

}

// Attach the current process to shared memory associated with file and return a pointer to it
void* dummy_shm_wrapper::attach_memory_block()
{
    //Get the block-id for the specific file
    int shared_block_id{ get_shared_block(block_size) };

    if (shared_block_id == SHM_ERROR)
    {
        return nullptr;
    }

    //map the shared block into this process's memory and give a pointer to it
    void* result{ (shmat(shared_block_id, nullptr, 0)) };
    if (result == reinterpret_cast<void*>(SHM_ERROR))
    {
        return nullptr;
    }

    return result;
}

// Detach the memory block from process, and return boolean if succesful or not
bool dummy_shm_wrapper::detach_memory_block()
{
    bool detachment{ shmdt(ptr_to_memory) != SHM_ERROR };
    if (!detachment)
    {
        std::cout << "Detachment failed: " << std::strerror(errno) << '\n';   
    }
    return detachment;
}

// Destroy the shared memory block, and return a boolean if succesful or notz
bool dummy_shm_wrapper::destroy_memory_block()
{
    int shared_block_id{ get_shared_block(0) };

    if (shared_block_id == SHM_ERROR)
    {
        std::cerr << "Destroy: invalid shared_block_id for file " << filename << '\n';
        return false;
    }

    return (shmctl(shared_block_id, IPC_RMID, nullptr) != SHM_ERROR);
}


constexpr float CONVEYOR_HEAT_CONSTANT{ 3.6 };
constexpr float CONVEYOR_MAX_EFFICIENCY{ 0.44 };
constexpr int16_t HEATER_ON { 10 };
constexpr int16_t HEATER_OFF{ -5 };
constexpr int16_t COOLER_ON{ -10 };
constexpr uint16_t QC_CAMERA_FAILS{ 0 };

//A simple function to modify each
void modify_temperature(sensor_data& dummy_data, int16_t modifier)
{
    dummy_data.temp_sensor01 += modifier;
    dummy_data.temp_sensor02 += modifier;
    dummy_data.temp_sensor03 += modifier;
    dummy_data.temp_sensor04 += modifier;
    dummy_data.temp_sensor05 += modifier;
    dummy_data.temp_sensor06 += modifier;
    dummy_data.temp_sensor07 += modifier;
    dummy_data.temp_sensor08 += modifier;
    dummy_data.temp_sensor09 += modifier;
    dummy_data.temp_sensor10 += modifier;
}

//A function to simulate the conveyors behaviour, and it's affects on temperature and speed
void simulate_conveyor(sensor_data& dummy_data, const control_data& ctrl_data)
{
    // Check if control_data speed is set higher than sensor speed, raise sensor speed
    if (ctrl_data.speed_of_conveyor > dummy_data.speed_of_conveyor)
    {
        ++dummy_data.speed_of_conveyor;
    }
    // Check if control_data speed is lower than sensor speed, lower sensor speed
    else if (ctrl_data.speed_of_conveyor < dummy_data.speed_of_conveyor)
    {
        --dummy_data.speed_of_conveyor;
    }
    
    // Check if speed is under 10% (ie. 25) then calculate the efficiency (linear from 44% to 0%)
    // If speed is over 10% (ie. 25 or over) efficiency is constant with speed 44%
    if (dummy_data.speed_of_conveyor < 25)
    {
        float temperature_modifier{ (1.0f - (CONVEYOR_MAX_EFFICIENCY * static_cast<float>(dummy_data.speed_of_conveyor) / 25.0f))
        * CONVEYOR_HEAT_CONSTANT * static_cast<float>(dummy_data.speed_of_conveyor) };
        modify_temperature(dummy_data, static_cast<int16_t>(temperature_modifier));
    }
    else
    {
        float temperature_modifier{ (1.0f - CONVEYOR_MAX_EFFICIENCY) * CONVEYOR_HEAT_CONSTANT * static_cast<float>(dummy_data.speed_of_conveyor) };
        modify_temperature(dummy_data, static_cast<int16_t>(temperature_modifier));
    }

}

//Simple data_generator
 void dummy_data_generator(sensor_data& dummy_data, const control_data& ctrl_data) {
    if (ctrl_data.heaters & HEATER_1)
    {
        modify_temperature(dummy_data, HEATER_ON);
    }
    else
    {
        modify_temperature(dummy_data, HEATER_OFF);
    }
    if (ctrl_data.heaters & HEATER_2)
    {
        modify_temperature(dummy_data, HEATER_ON);
    }
    else
    {
        modify_temperature(dummy_data, HEATER_OFF);
    }
    if (ctrl_data.heaters & HEATER_3)
    {
        modify_temperature(dummy_data, HEATER_ON);
    }
    else
    {
        modify_temperature(dummy_data, HEATER_OFF);
    }
    if (ctrl_data.cooler == 1)
    {
        modify_temperature(dummy_data, COOLER_ON);
    }

    simulate_conveyor(dummy_data, ctrl_data);

    dummy_data.qc_camera_fails = QC_CAMERA_FAILS;

 }

 int main()
 {
    dummy_shm_wrapper dummy_data_block(std::string{ "dummy_smh" });
    sensor_data sensor_input{ 0, 0, 250, 250, 250, 250, 250, 250, 250, 250, 250, 250, 0 };
    control_data ctrl_data{0, 0, 0, 0};
    while (!dummy_data_block.quit())
    {
        dummy_data_generator(sensor_input, ctrl_data);

        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor01, 1);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor02, 2);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor03, 3);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor04, 4);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor05, 5);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor06, 6);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor07, 7);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor08, 8);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor09, 9);
        dummy_data_block.set_temperature_sensor(sensor_input.temp_sensor10, 10);

        dummy_data_block.set_conveyor_speed_sensor(sensor_input.speed_of_conveyor);

        dummy_data_block.set_qc_camera_feed(sensor_input.qc_camera_fails);

        std::cout << "Data set\n";

        ctrl_data.speed_of_conveyor = dummy_data_block.read_conveyor_target_speed();
        ctrl_data.heaters = dummy_data_block.read_heaters();
        ctrl_data.cooler = dummy_data_block.read_cooler();
        ctrl_data.camera_toggle = dummy_data_block.read_camera_status();

        std::cout << "Data read\n\n";
    }

    std::cout << "Loop ended, and application quit, because of call from other process.\n";
    
    return 0;
 }
