submodule (pathlib) no_cpp_fs
!! stub for non-C++17 filesystem

implicit none (type, external)

contains

module procedure exists
error stop "pathlib: exists() requires C++17 filesystem"
end procedure exists

module procedure is_symlink
error stop "pathlib: is_symlink() requires C++17 filesystem"
end procedure is_symlink

module procedure create_symlink
error stop "pathlib: create_symlink() requires C++17 filesystem"
end procedure create_symlink


end submodule no_cpp_fs
