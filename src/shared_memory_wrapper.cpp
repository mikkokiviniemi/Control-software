#include "shared_memory_wrapper.hpp"

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;


simulation_shm_wrapper::simulation_shm_wrapper(const std::string& new_filename, std::size_t new_block_size)
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

    // Set the state to be read by other process
    running(true);
}

// Read temperature sensor ranging from 1 to 10, return -999 if wrong indexing
int16_t simulation_shm_wrapper::read_temperature_sensor(uint8_t sensor_number) const
{
    if ( sensor_number < 1 || sensor_number > 10 )
    {
        std::cerr << "Faulty index, returning -999\n";
        return -999;
    }
    return *(reinterpret_cast<int16_t*>(value_ptr + (sensor_number - 1) * 2));
}

uint8_t simulation_shm_wrapper::read_conveyor_speed_sensor() const
{
    return *(value_ptr + 20);
}

uint16_t simulation_shm_wrapper::read_qc_camera_feed() const
{
    return *(reinterpret_cast<uint16_t*>(value_ptr + 25));
}

void simulation_shm_wrapper::set_conveyor_target_speed(uint8_t target_speed)
{
    *(value_ptr + 21) = target_speed;
}

void simulation_shm_wrapper::set_heaters(uint8_t heaters)
{
    *(value_ptr + 22) = heaters;
}

void simulation_shm_wrapper::set_cooler(uint8_t cooler)
{
    *(value_ptr + 23) = cooler;
}

void simulation_shm_wrapper::set_camera_status(uint8_t camera_status)
{
    *(value_ptr + 24) = camera_status;
}

void simulation_shm_wrapper::running(bool value)
{
    if (value)
    {
        *(value_ptr + 27) = 0;
    }
    else
    {
        *(value_ptr + 27) = 1;
    }
}

void simulation_shm_wrapper::read_sensor_inputs(sensor_data& sensor_input) const
{
    sensor_input.temp_sensor01 = read_temperature_sensor(1);
    sensor_input.temp_sensor02 = read_temperature_sensor(2);
    sensor_input.temp_sensor03 = read_temperature_sensor(3);
    sensor_input.temp_sensor04 = read_temperature_sensor(4);
    sensor_input.temp_sensor05 = read_temperature_sensor(5);
    sensor_input.temp_sensor06 = read_temperature_sensor(6);
    sensor_input.temp_sensor07 = read_temperature_sensor(7);
    sensor_input.temp_sensor08 = read_temperature_sensor(8);
    sensor_input.temp_sensor09 = read_temperature_sensor(9);
    sensor_input.temp_sensor10 = read_temperature_sensor(10);

    sensor_input.speed_of_conveyor = read_conveyor_speed_sensor();
    sensor_input.qc_camera_fails = read_qc_camera_feed();

}

void simulation_shm_wrapper::set_control_data(control_data& ctrl_data)
{
    set_conveyor_target_speed(ctrl_data.speed_of_conveyor);
    set_heaters(ctrl_data.heaters);
    set_cooler(ctrl_data.cooler);
    set_camera_status(ctrl_data.camera_toggle);
}


simulation_shm_wrapper::~simulation_shm_wrapper()
{
    //Give signal to other process to quit
    running(false);
    
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
int simulation_shm_wrapper::get_shared_block(std::size_t size)
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
void* simulation_shm_wrapper::attach_memory_block()
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
bool simulation_shm_wrapper::detach_memory_block()
{
    bool detachment{ shmdt(ptr_to_memory) != SHM_ERROR };
    if (!detachment)
    {
        std::cout << "Detachment failed: " << std::strerror(errno) << '\n';   
    }
    return detachment;
}

// Destroy the shared memory block, and return a boolean if succesful or notz
bool simulation_shm_wrapper::destroy_memory_block()
{
    int shared_block_id{ get_shared_block(0) };

    if (shared_block_id == SHM_ERROR)
    {
        std::cerr << "Destroy: invalid shared_block_id for file " << filename << '\n';
        return false;
    }

    return (shmctl(shared_block_id, IPC_RMID, nullptr) != SHM_ERROR);
}
