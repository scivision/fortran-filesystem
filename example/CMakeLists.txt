cmake_minimum_required(VERSION 3.15...3.25)

project(ffilesystemExample LANGUAGES C)

enable_testing()

find_package(ffilesystem CONFIG REQUIRED)
message(STATUS "ffilesystem found: ${ffilesystem_DIR}")

if(ffilesystem_cpp)
  enable_language(CXX)
endif()

if(ffilesystem_fortran)
  enable_language(Fortran)
endif()

include(${PROJECT_SOURCE_DIR}/../cmake/DllTestPath.cmake)

set(CMAKE_CXX_STANDARD 17)

# --- Fortran use
if(ffilesystem_fortran)

add_executable(ex_fortran 1/main.f90)
target_link_libraries(ex_fortran PRIVATE ffilesystem::filesystem)
set_property(TARGET ex_fortran PROPERTY LINKER_LANGUAGE Fortran)
# Intel needs linker_language Fortran else error "undefined reference to `main'"
add_test(NAME BasicFortran COMMAND ex_fortran)

endif()

# --- C++ use
if(ffilesystem_cpp)

add_executable(ex_cpp 2/main.cpp)
target_link_libraries(ex_cpp PRIVATE ffilesystem::filesystem)
add_test(NAME BasicCpp COMMAND ex_cpp)

endif()

# --- C use
add_executable(ex_c 3/main.c)
target_link_libraries(ex_c PRIVATE ffilesystem::filesystem)
add_test(NAME BasicC COMMAND ex_c)


# --- test properties
get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

dll_test_path(ffilesystem::filesystem "${test_names}")

file(GENERATE OUTPUT .gitignore CONTENT "*")
