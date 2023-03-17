cmake_minimum_required(VERSION 3.20)

execute_process(COMMAND ${exe} "${exe}/."
OUTPUT_VARIABLE out OUTPUT_STRIP_TRAILING_WHITESPACE
)

cmake_path(COMPARE "${exe}" EQUAL "${out}" ok)

if(NOT ok)
  message(FATAL_ERROR "${exe} is not lexigraphically equal to ${out}")
endif()
