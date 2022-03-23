cmake_minimum_required(VERSION 3.21...3.23)

project(FilesystemExample
LANGUAGES C CXX Fortran
)

enable_testing()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE PATH "Install top-level directory" FORCE)
endif()

include(${PROJECT_SOURCE_DIR}/../cmake/ffilesystem.cmake)

# --- example 1
add_executable(example1 ex1.f90)
target_link_libraries(example1 PRIVATE ffilesystem::filesystem)
if(CMAKE_CXX_COMPILER_ID MATCHES "^Intel")
  # Intel needs linker_language fortran else error "undefined reference to `main'"
  set_target_properties(example1 PROPERTIES LINKER_LANGUAGE Fortran)
else()
  # setting linker language ensures stdc++ is linked, else
  # get hundreds of linker errors
  set_target_properties(example1 PROPERTIES LINKER_LANGUAGE CXX)
endif()

add_test(NAME Example1 COMMAND example1)
set_tests_properties(Example1 PROPERTIES
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
TIMEOUT 10
)

# --- Windows shared DLLs
if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET example1 POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:example1> $<TARGET_FILE_DIR:example1>
  COMMAND_EXPAND_LISTS
  )
endif()
