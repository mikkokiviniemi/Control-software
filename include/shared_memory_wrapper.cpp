#ifndef SHARED_MEMORY_WRAPPER_CPP
#define SHARED_MEMORY_WRAPPER_CPP

#include <cstdint>
#include <string>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

constexpr int16_t SHM_ERROR{ -1 };

enum class Destroy_status
{
    true,
    false,
    invalid_block_id
};

static int get_shared_block(const std::string& filename, std::size_t size)
{
    key_t key;

    // Request a key that is linked to a filename, so that other programs can access it
    key = ftok(filename.c_str(), 0);
    if (key == SHM_ERROR)
    {
        return SHM_ERROR;
    }

    //get shared block-id (0666 read-write permissions for all). Create it if it doesn't exist

    return shmget(key, size, 0666 | IPC_CREAT);
}

uint8_t* attach_memory_block(const std::string& filename, std::size_t size)
{
    //Get the block-id for the specific file
    int shared_block_id{ get_shared_block(filename, size) };

    if (shared_block_id == SHM_ERROR)
    {
        return nullptr;
    }

    //map the shared block into this process's memory and give a pointer to it
    uint8_t* result = shmat(shared_block_id, nullptr, 0);
    if (result == reinterpret_cast<char*>(SHM_ERROR))
    {
        return nullptr;
    }

    return result;
}

//Detach the memory block from process, and return boolean if succesful or not
bool detach_memory_block(uint8_t* block)
{
    return (shmdt(block) != SHM_ERROR);
}

bool destroy_memory_block(uint8_t* filename)
{
    int shared_block_id{ get_shared_block(filename, 0) };

    if (shared_block_id == SHM_ERROR)
    {
        std::cerr << "Invalid shared_block_id for file " << filename << '\n';
        return false;
    }

    return (shmctl(shared_block_id, IPC_RMID, nullptr) != SHM_ERROR);
}

class simulation_shm_wrapper
{
public:
    simulation_shm_wrapper(const std::string& new_filename, std::size_t new_block_size = 4096)
        : filename{ new_filename }, block_size{ new_block_size }, ptr_to_memory{attach_memory_block(filename, block_size)}
    {}
    
    int16_t read_temperature_sensor(uint8_t index)
    {
        return (static_cast<int16_t>(*(ptr_to_memory + index * 2)) << 8) | *(ptr_to_memory + index * 2 + 1);
    }
    uint8_t read_conveyor_speed_sensor()
    {
        return *(ptr_to_memory + 20);
    }
    uint16_t read_qc_camera_feed();
    {
        return (static_cast<int16_t>(*(ptr_to_memory + 25)) << 8) | *(ptr_to_memory + 26);
    }
    void set_conveyor_target_speed(uint8_t target_speed)
    {
        *(ptr_to_memory + 21) = target_speed;
    }
    
    void set_heaters(uint8_t heaters)
    {
        *(ptr_to_memory + 22) = heaters;
    }
    void set_cooler(uint8_t cooler)
    {
        *(ptr_to_memory + 23) = cooler;
    }
    void set_camera_status(uint8_t camera_status)
    {
        *(ptr_to_memory + 24) = camera_status;
    }
    
    ~simulation_shm_wrapper()
    {
        if (detach_memory_block)
        {
            std::cout << filename << " memory_block detached succesfully\n";
        }
        else
            std::cout << filename << " memory_block detachment unsuccesfull\n";
    }


private:
    std::string filename;
    std::size_t block_size;
    char* ptr_to_memory;

};


#endif
