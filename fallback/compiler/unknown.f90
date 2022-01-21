submodule (pathlib) compiler_unknown
!! error routine for non-Intel, non-GCC.

implicit none (type, external)

character(*), parameter :: tail = " not supported on this compiler / system."

contains
module procedure is_dir
error stop "pathlib: is_dir " // tail
end procedure is_dir

module procedure get_cwd
error stop "pathlib: get_cwd() " // tail
end procedure get_cwd

module procedure file_size
error stop "pathlib: %file_size() " // tail
end procedure file_size

end submodule pathlib_dummy
