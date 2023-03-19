cmake_minimum_required(VERSION 3.20)

project(concept_exedir LANGUAGES C)

file(GENERATE OUTPUT .gitignore CONTENT "*")

add_executable(exec exec.c)
target_compile_definitions(exec PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)

if(WIN32)
  add_executable(CreateProcess CreateProcess.c)
  target_compile_definitions(CreateProcess PRIVATE $<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>)
endif()
