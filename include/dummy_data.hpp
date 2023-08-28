#ifndef DUMMY_DATA_HPP
#define DUMMY_DATA_HPP

#include "data_structures.hpp"

#include <cstdint>
#include <iostream>
#include <string>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

constexpr int16_t SHM_ERROR{ -1 };

void dummy_data_generator(sensor_data& dummy_data, const control_data& ctrl_data);

class dummy_shm_wrapper
{
public:

    dummy_shm_wrapper(const std::string& new_filename, std::size_t new_block_size = 4096);
    ~dummy_shm_wrapper();

    dummy_shm_wrapper(const dummy_shm_wrapper&) = delete;
    dummy_shm_wrapper(dummy_shm_wrapper&) = default;

    dummy_shm_wrapper& operator=(const dummy_shm_wrapper&) = delete;
    dummy_shm_wrapper& operator=(dummy_shm_wrapper&&) = default;

    void set_temperature_sensor(int16_t value, uint8_t index);
    void set_conveyor_speed_sensor(uint8_t value);
    void set_qc_camera_feed(uint16_t value);

    uint8_t read_conveyor_target_speed() const;
    uint8_t read_heaters() const;
    uint8_t read_cooler() const;
    uint8_t read_camera_status() const;
    bool quit() const;


private:
    std::string filename;
    std::size_t block_size;
    void* ptr_to_memory;
    uint8_t* value_ptr;

    int get_shared_block(std::size_t size);
    void* attach_memory_block();
    
    //Detach the memory block from process, and return boolean if succesful or not
    bool detach_memory_block();
    bool destroy_memory_block();

};


#endif
