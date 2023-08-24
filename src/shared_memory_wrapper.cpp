#include "shared_memory_wrapper.hpp"



simulation_shm_wrapper::simulation_shm_wrapper(const std::string& new_filename, std::size_t new_block_size = 4096)
    : filename{ new_filename }, block_size{ new_block_size }
{
    ptr_to_memory = attach_memory_block();
}

int16_t simulation_shm_wrapper::read_temperature_sensor(uint8_t index)
{
    return *(reinterpret_cast<int16_t*>(ptr_to_memory + index * 2));
    // return (static_cast<int16_t>(*(ptr_to_memory + index * 2)) << 8) | *(ptr_to_memory + index * 2 + 1);
}
uint8_t simulation_shm_wrapper::read_conveyor_speed_sensor()
{
    return *(reinterpret_cast<uint8_t*>(ptr_to_memory + 20));
    // return *(ptr_to_memory + 20);
}
uint16_t simulation_shm_wrapper::read_qc_camera_feed()
{
    return *(reinterpret_cast<uint16_t*>(ptr_to_memory + 25));
    // return (static_cast<int16_t>(*(ptr_to_memory + 25)) << 8) | *(ptr_to_memory + 26);
}
void simulation_shm_wrapper::set_conveyor_target_speed(uint8_t target_speed)
{
    *(reinterpret_cast<uint8_t*>(ptr_to_memory + 21)) = target_speed;
    // *(ptr_to_memory + 21) = target_speed;
}

void simulation_shm_wrapper::set_heaters(uint8_t heaters)
{
    *(reinterpret_cast<uint8_t*>(ptr_to_memory + 22)) = heaters;
    // *(ptr_to_memory + 22) = heaters;
}
void simulation_shm_wrapper::set_cooler(uint8_t cooler)
{
    *(reinterpret_cast<uint8_t*>(ptr_to_memory + 23)) = cooler;
    // *(ptr_to_memory + 23) = cooler;
}
void simulation_shm_wrapper::set_camera_status(uint8_t camera_status)
{
    *(reinterpret_cast<uint8_t*>(ptr_to_memory + 24)) = camera_status;
    // *(ptr_to_memory + 24) = camera_status;
}

simulation_shm_wrapper::~simulation_shm_wrapper()
{
    if (master)
    {
        destroy_memory_block();
    }
    if (detach_memory_block())
    {
        std::cout << filename << " memory_block detached succesfully\n";
    }
    else
        std::cout << filename << " memory_block detachment unsuccesfull\n";
}

int simulation_shm_wrapper::get_shared_block(std::size_t size)
{
    key_t key;

    // Request a key that is linked to a filename, so that other programs can access it
    key = ftok(filename.c_str(), 0);
    if (key == SHM_ERROR)
    {
        return SHM_ERROR;
    }

    //get shared block-id (0666 read-write permissions for all). Create it if it doesn't exist
    int shared_block_id{ shmget(key, size, 0666) };
    if (shared_block_id == ENOENT)
    {
        master = true;
        shared_block_id = shmget(key, size, 0666 | IPC_CREAT);
    }
    else
    {
        master = false;
    }
    return shared_block_id;
}

uint8_t* simulation_shm_wrapper::attach_memory_block()
{
    //Get the block-id for the specific file
    int shared_block_id{ get_shared_block(block_size) };

    if (shared_block_id == SHM_ERROR)
    {
        return nullptr;
    }

    //map the shared block into this process's memory and give a pointer to it
    uint8_t* result{ reinterpret_cast<uint8_t*>(shmat(shared_block_id, nullptr, 0)) };
    if (result == reinterpret_cast<uint8_t*>(SHM_ERROR))
    {
        return nullptr;
    }

    return result;
}

//Detach the memory block from process, and return boolean if succesful or not
bool simulation_shm_wrapper::detach_memory_block()
{
    return (shmdt(ptr_to_memory) != SHM_ERROR);
}

bool simulation_shm_wrapper::destroy_memory_block()
{
    int shared_block_id{ get_shared_block(0) };

    if (shared_block_id == SHM_ERROR)
    {
        std::cerr << "Invalid shared_block_id for file " << filename << '\n';
        return false;
    }

    return (shmctl(shared_block_id, IPC_RMID, nullptr) != SHM_ERROR);
}


