# --- C++17 filesystem
add_library(filesystem_cpp OBJECT filesystem.cpp)
target_include_directories(filesystem_cpp PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(filesystem_cpp PRIVATE cxx_std_17)
target_compile_definitions(filesystem_cpp PRIVATE
$<$<BOOL:${MSVC}>:_CRT_SECURE_NO_WARNINGS>
)

#--- optional lib_path()
if(WIN32)
  target_compile_definitions(filesystem_cpp PRIVATE FS_DLL_NAME="$<TARGET_FILE_BASE_NAME:filesystem>.dll")
elseif(HAVE_DLADDR)
  target_link_libraries(filesystem_cpp PRIVATE ${CMAKE_DL_LIBS})
  target_compile_definitions(filesystem_cpp PRIVATE _GNU_SOURCE HAVE_DLADDR)
endif()

# --- source files
set(s ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(filesystem PRIVATE
${s}/filesystem.f90 ${s}/iter.f90 ${s}/io.f90 ${s}/find.f90 ${s}/fs_cpp.f90
$<TARGET_OBJECTS:filesystem_cpp>
)
