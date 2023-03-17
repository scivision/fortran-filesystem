cmake_minimum_required(VERSION 3.20)

set(link ${exe}_link)
file(CREATE_LINK ${exe} ${link} SYMBOLIC)

execute_process(COMMAND ${exe} "${link}"
OUTPUT_VARIABLE out OUTPUT_STRIP_TRAILING_WHITESPACE
)

cmake_path(COMPARE "${exe}" EQUAL "${out}" ok)

if(NOT ok)
  message(FATAL_ERROR "canonical(${exe}) does not lexicographically equal ${out}")
endif()
