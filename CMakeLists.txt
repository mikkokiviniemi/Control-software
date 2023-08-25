cmake_minimum_required(VERSION 3.20)

project(control_software LANGUAGES CXX)

# add_compile_options(-Wall -Wextra -Wpedantic)
# set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(main main.cpp)

# Add paths to Paho MQTT header files and libraries
set(PAHO_MQTT_C_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/paho.mqtt.c/")
set(PAHO_MQTT_CPP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/paho.mqtt.cpp/")

# Add PahoMQTT subdirectories
add_subdirectory(${PAHO_MQTT_C_DIR})
add_subdirectory(${PAHO_MQTT_CPP_DIR})

add_subdirectory(src)
target_link_libraries(main text_ui json_output dummy_data mqtt_client)



# add_subdirectory(tests)
