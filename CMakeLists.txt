cmake_minimum_required(VERSION 3.20)

project(control_software LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(first main.cpp)
add_executable(second shm_test.cpp) 

add_subdirectory(src)
target_link_libraries(first text_ui json_output dummy_data automatic_controls)

target_link_libraries(second text_ui json_output dummy_data automatic_controls)
add_subdirectory(tests)
