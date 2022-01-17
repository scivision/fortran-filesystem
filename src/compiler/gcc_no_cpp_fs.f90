submodule (pathlib) gcc_no_cpp_fs
!! GCC non-C++17 filesystem

implicit none (type, external)

contains

module procedure f_unlink
intrinsic :: unlink
call unlink(path)
end procedure f_unlink


end submodule gcc_no_cpp_fs
