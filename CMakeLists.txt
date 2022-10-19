cmake_minimum_required(VERSION 3.11...3.25)

project(ffilesystem
LANGUAGES C
VERSION 4.1.1
)

include(CTest) # needed for "ctest -T memcheck"

set(CMAKE_CXX_STANDARD 17)

include(cmake/options.cmake)
if(cpp)
  enable_language(CXX)
endif()
if(fortran)
  enable_language(Fortran)
endif()

include(cmake/compilers.cmake)

# --- path finding
add_library(get_path OBJECT src/get_path.c)
target_include_directories(get_path PRIVATE include)
target_compile_definitions(get_path PRIVATE _GNU_SOURCE)
if(BUILD_SHARED_LIBS)
  if(WIN32)
    target_compile_definitions(get_path PRIVATE FS_DLL_NAME="$<TARGET_FILE_NAME:ffilesystem>")
  elseif(HAVE_DLADDR)
    target_link_libraries(get_path PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(get_path PRIVATE HAVE_DLADDR)
  endif()
endif()

# --- filesystem library

add_library(ffilesystem src/common.c
$<TARGET_OBJECTS:get_path>
)
target_compile_definitions(ffilesystem PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)
set_property(TARGET ffilesystem PROPERTY PUBLIC_HEADER include/ffilesystem.h)
set_property(TARGET ffilesystem PROPERTY EXPORT_NAME filesystem)

if(fortran)
  target_sources(ffilesystem PRIVATE
  src/get_path.f90
  src/filesystem.f90 src/f2c.f90 src/find.f90
  )
endif()

if(HAVE_CXX_FILESYSTEM)
  add_library(filesystem_cpp OBJECT src/filesystem.cpp)
  target_include_directories(filesystem_cpp PRIVATE ${PROJECT_SOURCE_DIR}/include)
  target_compile_definitions(filesystem_cpp PRIVATE
  $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
  )

  target_sources(ffilesystem PRIVATE $<TARGET_OBJECTS:filesystem_cpp>)
else()
  add_library(filesystem_c OBJECT src/c/filesystem.c src/c/env.c src/c/sys.c
  src/c/cwalk.c
  )
  target_include_directories(filesystem_c PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/src/c
  )
  target_compile_definitions(filesystem_c PRIVATE
  $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
  _GNU_SOURCE
  CWK_EXPORTS
  )

  target_sources(ffilesystem PRIVATE $<TARGET_OBJECTS:filesystem_c>)
endif()

set_property(TARGET ffilesystem PROPERTY Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
set_property(TARGET ffilesystem PROPERTY RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set_property(TARGET ffilesystem PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set_property(TARGET ffilesystem PROPERTY LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

target_include_directories(ffilesystem PUBLIC
$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(ffilesystem
INTERFACE
${GNU_stdfs}
$<$<BOOL:${HAVE_DLADDR}>:${CMAKE_DL_LIBS}>
)

if(fortran)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/filesystem.mod DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

install(TARGETS ffilesystem EXPORT ${PROJECT_NAME}-targets
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_library(ffilesystem::filesystem INTERFACE IMPORTED)

target_link_libraries(ffilesystem::filesystem INTERFACE ffilesystem)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.19)
  set_property(TARGET ffilesystem::filesystem PROPERTY IMPORTED_LINK_INTERFACE_LANGUAGE CXX)
  # https://cmake.org/cmake/help/latest/prop_tgt/IMPORTED_LINK_INTERFACE_LANGUAGES.html
  # imported targets use above instead of LINKER_LANGUAGE
  # target_link_libraries(ffilesystem::filesystem INTERFACE stdc++)  # did not help
endif()

# --- CLI

if(cli)
  add_executable(fs_cli app/main.c)
  target_compile_definitions(fs_cli PRIVATE "$<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>")
  target_link_libraries(fs_cli PRIVATE ffilesystem)

  if(fortran)
  add_executable(filesystem_cli app/main.f90)
  target_link_libraries(filesystem_cli PRIVATE ffilesystem)
  set_property(TARGET filesystem_cli PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_property(TARGET filesystem_cli PROPERTY LINKER_LANGUAGE Fortran)
  # Intel/NVHPC needs linker_language fortran else error "undefined reference to `main'"

  install(TARGETS filesystem_cli EXPORT ${PROJECT_NAME}-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
  endif()
endif()

# --- Tests
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

include(cmake/install.cmake)

include(FeatureSummary)

add_feature_info(Fortran fortran "Fortran bindings")
add_feature_info(CXX_FILESYSTEM HAVE_CXX_FILESYSTEM "C++17 filesystem")
add_feature_info(shared BUILD_SHARED_LIBS "Build shared library: enables lib_path(), lib_dir()")

feature_summary(WHAT ALL)
