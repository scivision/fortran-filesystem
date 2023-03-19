cmake_minimum_required(VERSION 3.20)

project(file_types LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

add_executable(file_types main.cpp file_status.cpp)
target_compile_definitions(file_types PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)
target_include_directories(file_types PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(file_types PRIVATE "$<$<BOOL:${WIN32}>:wsock32;ws2_32>") # Iphlpapi)
add_test(NAME file_types COMMAND file_types)

file(GENERATE OUTPUT .gitignore CONTENT "*")
