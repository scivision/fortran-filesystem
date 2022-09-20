cmake_minimum_required(VERSION 3.8...3.25)

project(FilesystemExample
LANGUAGES C CXX Fortran
)

enable_testing()

find_package(ffilesystem CONFIG REQUIRED)

# --- Fortran use of our custom C++ functions
add_executable(ex_fortran ex1.f90)
target_link_libraries(ex_fortran PRIVATE ffilesystem::filesystem)
set_property(TARGET ex_fortran PROPERTY LINKER_LANGUAGE Fortran)
# Intel needs linker_language fortran else error "undefined reference to `main'"
add_test(NAME ExFortran COMMAND ex_fortran)

if(filesystem_cpp)
  # --- C++ use of our custom C++ functions
  add_executable(ex_cpp ex2.cpp)
  target_compile_features(ex_cpp PRIVATE cxx_std_17)
  target_link_libraries(ex_cpp PRIVATE ffilesystem::filesystem)

  add_test(NAME ExCpp COMMAND ex_cpp)

  # --- C use of our custom C++ functions
  add_executable(ex_c ex3.c)
  target_link_libraries(ex_c PRIVATE ffilesystem::filesystem)

  add_test(NAME ExC COMMAND ex_c)
endif()

# --- test properties
get_property(test_names DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY TESTS)
set_property(TEST ${test_names} PROPERTY TIMEOUT 10)

# --- Windows shared DLLs
if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET ex_fortran POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:ex_fortran> $<TARGET_FILE_DIR:ex_fortran>
  COMMAND_EXPAND_LISTS
  )
endif()

# --- auto-ignore build directory
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()
