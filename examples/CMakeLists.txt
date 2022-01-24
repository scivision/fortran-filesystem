cmake_minimum_required(VERSION 3.20...3.22)

project(FilesystemExample
LANGUAGES C CXX Fortran
)

enable_testing()

include(ExternalProject)

find_package(ffilesystem CONFIG)
if(NOT ffilesystem_FOUND)

  if(NOT ffilesystem_ROOT)
    set(ffilesystem_ROOT ${PROJECT_BINARY_DIR})
  endif()

  if(BUILD_SHARED_LIBS)
    set(ffilesystem_LIBRARIES ${ffilesystem_ROOT}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}filesystem${CMAKE_SHARED_LIBRARY_SUFFIX})
  else()
    set(ffilesystem_LIBRARIES ${ffilesystem_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}filesystem${CMAKE_STATIC_LIBRARY_SUFFIX})
  endif()

  set(ffilesystem_INCLUDE_DIRS ${ffilesystem_ROOT}/include)

  set(ffilesystem_args
  -DCMAKE_INSTALL_PREFIX=${ffilesystem_ROOT}
  -DBUILD_TESTING:BOOL=false
  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
  )

  ExternalProject_Add(ffilesystem
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
  CMAKE_ARGS ${ffilesystem_args}
  CONFIGURE_HANDLED_BY_BUILD ON
  BUILD_BYPRODUCTS ${ffilesystem_LIBRARIES}
  )

  file(MAKE_DIRECTORY ${ffilesystem_ROOT}/include)

  add_library(ffilesystem::filesystem INTERFACE IMPORTED GLOBAL)
  target_include_directories(ffilesystem::filesystem INTERFACE "${ffilesystem_INCLUDE_DIRS}")
  target_link_libraries(ffilesystem::filesystem INTERFACE "${ffilesystem_LIBRARIES}")

  add_dependencies(ffilesystem::filesystem ffilesystem)

endif()

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

add_test(NAME Example1
COMMAND $<TARGET_FILE:example1>
)
set_tests_properties(Example1 PROPERTIES
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
TIMEOUT 10
)
