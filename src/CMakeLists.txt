add_executable(dummy_data dummy_data.cpp)
target_include_directories(dummy_data PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(json_output json_output.cpp)
target_include_directories(json_output PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)

add_library(mqtt_client mqtt_client.cpp)
target_include_directories(mqtt_client PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)
target_link_libraries(mqtt_client paho-mqtt3a paho-mqttpp3)

add_library(mqtt_client_UI mqtt_client_UI.cpp)
target_include_directories(mqtt_client_UI PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)
target_link_libraries(mqtt_client_UI paho-mqtt3a paho-mqttpp3)

add_library(automatic_controls automatic_controls.cpp)
target_include_directories(automatic_controls PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(shared_memory_wrapper shared_memory_wrapper.cpp)
target_include_directories(shared_memory_wrapper PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(text_ui text_ui.cpp)
target_include_directories(text_ui PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(text_ui mqtt_client_UI)
