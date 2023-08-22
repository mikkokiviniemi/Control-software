add_library(text_ui text_ui.cpp)
target_include_directories(text_ui PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(json_output json_output.cpp)
target_include_directories(json_output PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/external)
