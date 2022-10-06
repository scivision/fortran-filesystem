cmake_minimum_required(VERSION 3.15...3.25)

project(ffilesystemExample
LANGUAGES C CXX Fortran
)

enable_testing()

find_package(ffilesystem CONFIG REQUIRED)

include(${PROJECT_SOURCE_DIR}/../cmake/DllTestPath.cmake)

set(CMAKE_CXX_STANDARD 17)

# --- Fortran use
add_executable(ex_fortran ex1.f90)
target_link_libraries(ex_fortran PRIVATE ffilesystem::filesystem)
set_property(TARGET ex_fortran PROPERTY LINKER_LANGUAGE Fortran)
# Intel needs linker_language fortran else error "undefined reference to `main'"
add_test(NAME BasicFortran COMMAND ex_fortran)

add_executable(test_binpath_f ${PROJECT_SOURCE_DIR}/../test/fortran/test_binpath.f90)
target_link_libraries(test_binpath_f PRIVATE ffilesystem::filesystem)
add_test(NAME binpath_fortran COMMAND test_binpath_f $<BOOL:${ffilesystem_shared}> $<TARGET_FILE_NAME:test_binpath_f>)

# --- C++ use
add_executable(ex_cpp ex2.cpp)
target_link_libraries(ex_cpp PRIVATE ffilesystem::filesystem)
add_test(NAME BasicCpp COMMAND ex_cpp)

add_executable(test_binpath_c ${PROJECT_SOURCE_DIR}/../test/c/test_binpath.c)
target_link_libraries(test_binpath_c PRIVATE ffilesystem::filesystem)
add_test(NAME binpath_c COMMAND test_binpath_c $<BOOL:${ffilesystem_shared}> $<TARGET_FILE_NAME:test_binpath_c>)

# --- C use
add_executable(ex_c ex3.c)
target_link_libraries(ex_c PRIVATE ffilesystem::filesystem)
add_test(NAME BasicC COMMAND ex_c)

add_executable(test_binpath_cpp ${PROJECT_SOURCE_DIR}/../test/cpp/test_binpath.cpp)
target_link_libraries(test_binpath_cpp PRIVATE ffilesystem::filesystem)
add_test(NAME binpath_cpp COMMAND test_binpath_cpp $<BOOL:${ffilesystem_shared}> $<TARGET_FILE_NAME:test_binpath_cpp>)


# --- test properties
get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)

# --- Windows shared DLLs
dll_test_path(ffilesystem::filesystem "${test_names}")

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()
