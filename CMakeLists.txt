cmake_minimum_required(VERSION 3.20)

project(control_software LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Wpedantic)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(main main.cpp)

# Find Paho C and C++ packages
find_package(eclipse-paho-mqtt-c REQUIRED)
find_package(PahoMqttCpp REQUIRED)

add_subdirectory(src)

target_link_libraries(main json_output mqtt_client automatic_controls shared_memory_wrapper failure_detection)

add_subdirectory(tests)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)
