add_library(filesystem_c OBJECT filesystem.c)
target_include_directories(filesystem_c PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_definitions(filesystem_c PRIVATE
$<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
)
target_compile_features(filesystem_c PRIVATE c_std_99)
target_compile_definitions(filesystem_c PRIVATE _GNU_SOURCE)

#--- sources
set(s ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(filesystem PRIVATE
${s}/filesystem.f90 ${s}/general.f90 ${s}/io.f90 ${s}/iter.f90 ${s}/f2c.f90 ${s}/find.f90
$<TARGET_OBJECTS:filesystem_c>
)
