# show that for C, we only need ffilesystem.h, filesystem.c, common.c, cwalk.c

cmake_minimum_required(VERSION 3.14...3.24)

project(C_standalone LANGUAGES C)

enable_testing()

include(FetchContent)

option(CMAKE_TLS_VERIFY "Enable TLS" on)

set(CMAKE_C_STANDARD 99)

set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(cwalk
URL https://github.com/likle/cwalk/archive/refs/tags/v1.2.9.tar.gz
)
if(NOT cwalk_POPULATED)
  FetchContent_Populate(cwalk)
endif()

add_library(ffilesystem
../../src/c/filesystem.c ../../src/common/common.c
${cwalk_SOURCE_DIR}/src/cwalk.c
)
target_include_directories(ffilesystem PUBLIC ../../include
PRIVATE ${cwalk_SOURCE_DIR}/include
)
target_compile_definitions(ffilesystem PRIVATE CWK_EXPORTS)

add_executable(main ../3/main.c)
target_link_libraries(main PRIVATE ffilesystem)

add_test(NAME C_standalone COMMAND main)

file(GENERATE OUTPUT .gitignore CONTENT "*")
