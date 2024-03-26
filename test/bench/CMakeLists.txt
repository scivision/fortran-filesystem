set_property(DIRECTORY PROPERTY LABELS "bench")


add_library(ffilesystem_bench OBJECT ffilesystem_bench.cpp)

target_include_directories(ffilesystem_bench PRIVATE ${PROJECT_SOURCE_DIR}/include
PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(ffilesystem_bench PRIVATE HAVE_CXX_FILESYSTEM=$<BOOL:${HAVE_CXX_FILESYSTEM}>)


foreach(t IN ITEMS canonical env exe expanduser)

add_executable(bench_${t} bench_${t}.cpp)
target_link_libraries(bench_${t} PRIVATE ffilesystem_bench ffilesystem)

add_test(NAME bench_${t} COMMAND bench_${t})
set_property(TEST bench_${t} PROPERTY RUN_SERIAL TRUE)

endforeach()
