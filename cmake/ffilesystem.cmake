include(ExternalProject)

if(NOT ffilesystem_external)
  find_package(ffilesystem CONFIG)

  if(ffilesystem_FOUND)
    message(STATUS "Fortran Filesystem found: ${ffilesystem_DIR}")
    return()
  endif()
endif()

set(ffilesystem_external true CACHE BOOL "Fortran Filesystem autobuild")

if(NOT ffilesystem_ROOT)
  set(ffilesystem_ROOT ${CMAKE_INSTALL_PREFIX})
endif()

if(BUILD_SHARED_LIBS)
  if(WIN32)
    set(ffilesystem_LIBRARIES ${ffilesystem_ROOT}/bin/${CMAKE_SHARED_LIBRARY_PREFIX}filesystem${CMAKE_SHARED_LIBRARY_SUFFIX})
  else()
    set(ffilesystem_LIBRARIES ${ffilesystem_ROOT}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}filesystem${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
else()
  set(ffilesystem_LIBRARIES ${ffilesystem_ROOT}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}filesystem${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

set(ffilesystem_INCLUDE_DIRS ${ffilesystem_ROOT}/include)

set(ffilesystem_cmake_args
-DCMAKE_INSTALL_PREFIX=${ffilesystem_ROOT}
-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
-DCMAKE_BUILD_TYPE=Release
-DBUILD_TESTING:BOOL=false
-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
-DCMAKE_Fortran_COMPILER=${CMAKE_Fortran_COMPILER}
)

ExternalProject_Add(FFILESYSTEM
SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
CMAKE_ARGS ${ffilesystem_cmake_args}
CMAKE_GENERATOR ${EXTPROJ_GENERATOR}
BUILD_BYPRODUCTS ${ffilesystem_LIBRARIES} ${ffs_implib}
INACTIVITY_TIMEOUT 15
CONFIGURE_HANDLED_BY_BUILD true
)

file(MAKE_DIRECTORY ${ffilesystem_INCLUDE_DIRS})
# avoid generate race condition

add_library(ffilesystem::filesystem INTERFACE IMPORTED)

target_link_libraries(ffilesystem::filesystem INTERFACE ${ffilesystem_LIBRARIES} ${lib_filesystem})
target_include_directories((ffilesystem::filesystem INTERFACE ${ffilesystem_INCLUDE_DIRS})
set_target_properties(ffilesystem::filesystem PROPERTIES LINKER_LANGUAGE CXX)
# target_link_libraries(ffilesystem::filesystem INTERFACE stdc++)  # did not help
# instead, set linker_langauge CXX for the specific targets linking ffilesystem::filesystem

add_dependencies(ffilesystem::filesystem FFILESYSTEM)
