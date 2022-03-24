cmake_minimum_required(VERSION 3.21...3.23)

project(FilesystemExample
LANGUAGES C CXX Fortran
)

enable_testing()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "Install top-level directory" FORCE)
endif()

include(${PROJECT_SOURCE_DIR}/../cmake/ffilesystem.cmake)

# --- Fortran use of our custom C++ functions
add_executable(ex_fortran ex1.f90)
target_link_libraries(ex_fortran PRIVATE ffilesystem::filesystem)
if(CMAKE_CXX_COMPILER_ID MATCHES "^Intel")
  # Intel needs linker_language fortran else error "undefined reference to `main'"
  set_target_properties(ex_fortran PROPERTIES LINKER_LANGUAGE Fortran)
else()
  # setting linker language ensures stdc++ is linked, else
  # get hundreds of linker errors
  set_target_properties(ex_fortran PROPERTIES LINKER_LANGUAGE CXX)
endif()

add_test(NAME ExFortran COMMAND ex_fortran)

# --- C++ use of our custom C++ functions
add_executable(ex_cpp ex2.cpp)
target_link_libraries(ex_cpp PRIVATE ffilesystem::filesystem)

add_test(NAME ExCpp COMMAND ex_cpp)

# --- C use of our custom C++ functions
add_executable(ex_c ex3.c)
target_link_libraries(ex_c PRIVATE ffilesystem::filesystem)
set_target_properties(ex_c PROPERTIES LINKER_LANGUAGE CXX)

add_test(NAME ExC COMMAND ex_c)

# --- test properties
set_tests_properties(ExFortran ExCpp ExC PROPERTIES
TIMEOUT 10
)

# --- Windows shared DLLs
if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET ex_fortran POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:ex_fortran> $<TARGET_FILE_DIR:ex_fortran>
  COMMAND_EXPAND_LISTS
  )
endif()
