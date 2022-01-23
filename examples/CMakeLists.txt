cmake_minimum_required(VERSION 3.19...3.22)

project(FilesystemExample
LANGUAGES C CXX Fortran
)

enable_testing()

find_package(FortranFilesystem CONFIG REQUIRED)

# --- example 1
add_executable(example1 ex1.f90)
target_link_libraries(example1 FortranFilesystem::filesystem)

add_test(NAME Example1
COMMAND $<TARGET_FILE:example1>
)
set_tests_properties(Example1 PROPERTIES
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
TIMEOUT 10
)
