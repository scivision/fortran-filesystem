cmake_minimum_required(VERSION 3.8...3.25)

project(ffilesystem
LANGUAGES C CXX Fortran
VERSION 3.8.0
)

include(CTest) # needed for "ctest -T memcheck"

include(cmake/options.cmake)
include(cmake/compilers.cmake)

# --- path finding
add_library(get_path OBJECT src/get_path.c)
target_include_directories(get_path PRIVATE include)
target_compile_features(get_path PRIVATE c_std_99)
if(WIN32)
  target_compile_definitions(get_path PRIVATE FS_DLL_NAME="$<TARGET_FILE_NAME:filesystem>")
elseif(HAVE_DLADDR)
  target_link_libraries(get_path PRIVATE ${CMAKE_DL_LIBS})
  target_compile_definitions(get_path PRIVATE _GNU_SOURCE HAVE_DLADDR)
endif()

# --- filesystem library

add_library(filesystem
src/max.c
src/get_path.f90 $<TARGET_OBJECTS:get_path>
)

if(fallback)
  add_subdirectory(src/fallback)
else()
  add_subdirectory(src)
endif()

set_property(TARGET filesystem PROPERTY Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
set_property(TARGET filesystem PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set_property(TARGET filesystem PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set_property(TARGET filesystem PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

target_include_directories(filesystem PUBLIC
$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(filesystem INTERFACE
${CMAKE_REQUIRED_LIBRARIES}
$<$<BOOL:${HAVE_DLADDR}>:${CMAKE_DL_LIBS}>
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/filesystem.mod DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/filesystem.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS filesystem EXPORT ${PROJECT_NAME}-targets
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_library(ffilesystem::filesystem INTERFACE IMPORTED)

target_link_libraries(ffilesystem::filesystem INTERFACE filesystem)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.19)
  set_property(TARGET ffilesystem::filesystem PROPERTY IMPORTED_LINK_INTERFACE_LANGUAGE CXX)
  # https://cmake.org/cmake/help/latest/prop_tgt/IMPORTED_LINK_INTERFACE_LANGUAGES.html
  # imported targets use above instead of LINKER_LANGUAGE
  # target_link_libraries(ffilesystem::filesystem INTERFACE stdc++)  # did not help
endif()

# --- CLI

if(BUILD_UTILS)
  add_executable(filesystem_cli app/main.f90)
  target_link_libraries(filesystem_cli PRIVATE filesystem)
  set_property(TARGET filesystem_cli PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_property(TARGET filesystem_cli PROPERTY LINKER_LANGUAGE Fortran)
  # Intel/NVHPC needs linker_language fortran else error "undefined reference to `main'"

  install(TARGETS filesystem_cli EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endif()

# --- Tests
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

include(cmake/install.cmake)
