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

char* attach_memory_block(const std::string& filename, std::size_t size)
{
    //Get the block-id for the specific file
    int shared_block_id{ get_shared_block(filename, size) };

    char* result;
    if (shared_block_id == SHM_ERROR)
    {
        return nullptr;
    }

    //map the shared block into this process's memory and give a pointer to it
    result = shmat(shared_block_id, nulltpr, 0);
    if (result == reinterpret_cast<char*>(SHM_ERROR))
    {
        return nullptr;
    }

    return result;
}

//Detach the memory block from process, and return boolean if succesful or not
bool detach_memory_block(char* block)
{
    return (shmdt(block) != SHM_ERROR);
}

bool destroy_memory_block(char* filename)
{
    int shared_block_id{ get_shared_block(filename, 0) };

    if (shared_block_id == SHM_ERROR)
    {
        std::cerr << "Invalid shared_block_id for file " << filename << '\n';
        return false;
    }

    return (shmctl(shared_block_id, IPC_RMID, nullptr) != SHM_ERROR);
}

class shm_wrapper
{
public:
    shm_wrapper(const std::string& filename, std::size_t block_size = 4096);
    
private:
    std::string filename;
    std::size_t mem_size;
    char* ptr_to_memory;

};

#endif
