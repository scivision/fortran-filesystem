submodule (filesystem) compiler_unknown
!! error routine for non-Intel, non-GCC.

implicit none (type, external)

character(*), parameter :: tail = " not supported on this compiler / system."

contains

module procedure get_cwd
get_cwd = ""
error stop "filesystem: get_cwd() " // tail
end procedure get_cwd


end submodule compiler_unknown
