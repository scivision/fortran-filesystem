include(ExternalProject)

if(BUILD_SHARED_LIBS)
  if(WIN32)
    set(ffilesystem_LIBRARIES ${CMAKE_INSTALL_FULL_BINDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}ffilesystem${CMAKE_SHARED_LIBRARY_SUFFIX})
  else()
    set(ffilesystem_LIBRARIES ${CMAKE_INSTALL_FULL_LIBDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}ffilesystem${CMAKE_SHARED_LIBRARY_SUFFIX})
  endif()
else()
  set(ffilesystem_LIBRARIES ${CMAKE_INSTALL_FULL_LIBDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}ffilesystem${CMAKE_STATIC_LIBRARY_SUFFIX})
endif()

set(ffilesystem_INCLUDE_DIRS ${CMAKE_INSTALL_FULL_INCLUDEDIR})

set(ffilesystem_cmake_args
-DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
-DCMAKE_BUILD_TYPE=Release
-Dffilesystem_BUILD_TESTING:BOOL=false
-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
-DCMAKE_Fortran_COMPILER=${CMAKE_Fortran_COMPILER}
)

ExternalProject_Add(ffilesystem
SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
CMAKE_ARGS ${ffilesystem_cmake_args}
BUILD_BYPRODUCTS ${ffilesystem_LIBRARIES}
CONFIGURE_HANDLED_BY_BUILD true
)

file(MAKE_DIRECTORY ${ffilesystem_INCLUDE_DIRS})
# avoid generate race condition

add_library(ffilesystem::filesystem INTERFACE IMPORTED)

target_link_libraries(ffilesystem::filesystem INTERFACE ${ffilesystem_LIBRARIES})
target_include_directories(ffilesystem::filesystem INTERFACE ${ffilesystem_INCLUDE_DIRS})
set_property(TARGET ffilesystem::filesystem PROPERTY IMPORTED_LINK_INTERFACE_LANGUAGES CXX)
# https://cmake.org/cmake/help/latest/prop_tgt/IMPORTED_LINK_INTERFACE_LANGUAGES.html
# imported targets use above instead of LINKER_LANGUAGE
# target_link_libraries(ffilesystem::filesystem INTERFACE stdc++)  # did not help

add_dependencies(ffilesystem::filesystem ffilesystem)
