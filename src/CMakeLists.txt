add_library(text_ui text_ui.cpp)
target_include_directories(text_ui PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(dummy_data dummy_data.cpp)
target_include_directories(dummy_data PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(json_output json_output.cpp)
target_include_directories(json_output PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)

add_library(mqtt_client mqtt_client.cpp)
target_include_directories(mqtt_client PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)
target_link_libraries(mqtt_client paho-mqtt3a paho-mqttpp3)
