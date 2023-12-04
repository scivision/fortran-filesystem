cmake_minimum_required(VERSION 3.15...3.28)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "Please use out of source build like:
  cmake -Bbuild")
endif()

project(ffilesystem
LANGUAGES C
VERSION 5.3.0
)

include(CTest) # needed for "ctest -T memcheck"
include(FetchContent)

set(CMAKE_C_STANDARD 11)

# some parts of ffilesystem have optional C++20 features
set(CMAKE_CXX_STANDARD 20)

message(STATUS "${PROJECT_NAME} ${PROJECT_VERSION} CMake ${CMAKE_VERSION} ${CMAKE_SYSTEM_NAME} Toolchain ${CMAKE_TOOLCHAIN_FILE}")

include(options.cmake)
if(ffilesystem_cpp)
  enable_language(CXX)
  message(STATUS "${PROJECT_NAME}  CXX_STANDARD ${CMAKE_CXX_STANDARD}")
else()
  message(STATUS "${PROJECT_NAME}  C_STANDARD ${CMAKE_C_STANDARD}")
endif()
if(ffilesystem_fortran)
  enable_language(Fortran)
endif()

include(cmake/compilers.cmake)

# CPPcheck
if(cppcheck)
  find_program(cppcheck_exe NAMES cppcheck REQUIRED)
  set(cppcheck_opts --enable=all --inline-suppr --quiet --suppressions-list=${PROJECT_SOURCE_DIR}/cppcheck.supp)
  set(CMAKE_C_CPPCHECK ${cppcheck_exe} --std=c++${CMAKE_CXX_STANDARD} ${cppcheck_opts})
  set(CMAKE_CXX_CPPCHECK ${cppcheck_exe} --std=c++${CMAKE_CXX_STANDARD} ${cppcheck_opts})
endif()
# --- filesystem library

if(NOT HAVE_CXX_FILESYSTEM)
  FetchContent_Declare(cwalk
  GIT_REPOSITORY https://github.com/likle/cwalk
  GIT_TAG v1.2.7
  TLS_VERIFY ${CMAKE_TLS_VERIFY}
  )
  FetchContent_Populate(cwalk)

endif()


add_library(ffilesystem src/common/common.c
"$<IF:$<BOOL:${HAVE_CXX_FILESYSTEM}>,src/common/filesystem.cpp,src/c/filesystem.c;src/c/env.c;src/c/get_path.c;src/c/sys.c;${cwalk_SOURCE_DIR}/src/cwalk.c>"
"$<$<BOOL:${ffilesystem_fortran}>:src/common/fortran/get_path.f90;src/common/fortran/filesystem.f90;src/common/fortran/f2c.f90;src/common/fortran/find.f90>"
)
target_include_directories(ffilesystem PRIVATE
${PROJECT_SOURCE_DIR}/include
$<$<NOT:$<BOOL:${HAVE_CXX_FILESYSTEM}>>:${cwalk_SOURCE_DIR}/include>
)
target_compile_definitions(ffilesystem PRIVATE
"$<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>"
"$<$<NOT:$<BOOL:${HAVE_CXX_FILESYSTEM}>>:CWK_EXPORTS>"
"$<$<BOOL:${HAVE_UTSNAME_H}>:HAVE_UTSNAME_H>"
"$<$<BOOL:${${PROJECT_NAME}_WIN32_SYMLINK}>:WIN32_SYMLINK>"
)
set_property(TARGET ffilesystem PROPERTY PUBLIC_HEADER include/ffilesystem.h)
set_property(TARGET ffilesystem PROPERTY EXPORT_NAME filesystem)
set_property(TARGET ffilesystem PROPERTY Fortran_MODULE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
set_property(TARGET ffilesystem PROPERTY VERSION ${PROJECT_VERSION})
if(BUILD_SHARED_LIBS)
  if(WIN32 OR CYGWIN)
    target_compile_definitions(ffilesystem PRIVATE FS_DLL_NAME="$<TARGET_FILE_NAME:ffilesystem>")
  elseif(HAVE_DLADDR)
    target_link_libraries(ffilesystem PRIVATE ${CMAKE_DL_LIBS})
    target_compile_definitions(ffilesystem PRIVATE HAVE_DLADDR)
  endif()
endif()

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include)
# include -Dffilesystem_fortran=off, the ffilesystem CMake package will fail on use if directory not exist

target_include_directories(ffilesystem PUBLIC
"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include;${PROJECT_SOURCE_DIR}/include>"
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(ffilesystem
INTERFACE
${GNU_stdfs}
$<$<BOOL:${HAVE_DLADDR}>:${CMAKE_DL_LIBS}>
"$<$<BOOL:${WIN32}>:Userenv;Advapi32>"
)
# both Advapi32 and Userenv must be linked

if(ffilesystem_fortran)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/filesystem.mod DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

install(TARGETS ffilesystem EXPORT ${PROJECT_NAME}-targets)

# GLOBAL for use from FetchContent
add_library(ffilesystem::filesystem INTERFACE IMPORTED GLOBAL)

target_link_libraries(ffilesystem::filesystem INTERFACE ffilesystem)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.19)
  set_property(TARGET ffilesystem::filesystem PROPERTY IMPORTED_LINK_INTERFACE_LANGUAGE CXX)
  # https://cmake.org/cmake/help/latest/prop_tgt/IMPORTED_LINK_INTERFACE_LANGUAGES.html
  # imported targets use above instead of LINKER_LANGUAGE
  # target_link_libraries(ffilesystem::filesystem INTERFACE stdc++)  # did not help
endif()

# --- CLI

if(ffilesystem_cli)
  if(HAVE_CXX_FILESYSTEM)
  add_executable(fs_cli app/main.cpp)
  target_compile_definitions(fs_cli PRIVATE "$<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>")
  target_link_libraries(fs_cli PRIVATE ffilesystem)

  install(TARGETS fs_cli EXPORT ${PROJECT_NAME}-targets)
  endif()

  if(ffilesystem_fortran)
  add_executable(filesystem_cli app/fortran/filesystem_cli.f90)
  target_link_libraries(filesystem_cli PRIVATE ffilesystem)
  set_property(TARGET filesystem_cli PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set_property(TARGET filesystem_cli PROPERTY LINKER_LANGUAGE Fortran)
  # Intel/NVHPC needs linker_language Fortran else error "undefined reference to `main'"

  install(TARGETS filesystem_cli EXPORT ${PROJECT_NAME}-targets)
  endif()
endif()

# --- Tests
if(BUILD_TESTING)
  add_subdirectory(test)
endif()

include(cmake/install.cmake)

include(FeatureSummary)

add_feature_info(Fortran ffilesystem_fortran "Fortran bindings")
add_feature_info(CXX_FILESYSTEM HAVE_CXX_FILESYSTEM "C++17 filesystem")
add_feature_info(shared BUILD_SHARED_LIBS "Build shared library: enables lib_path(), lib_dir()")

feature_summary(WHAT ALL)
