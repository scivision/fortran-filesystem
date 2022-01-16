submodule (pathlib) pathlib_dummy
!! generic routine for non-Intel, non-GCC.
!! better to make custom per-compiler routine based on pathlib_gcc for other compilers.

implicit none (type, external)

character(*), parameter :: tail = " not supported on this compiler / system."

contains


module procedure is_dir
error stop "pathlib: is_dir " // tail
end procedure is_dir

module procedure unlink
error stop "pathlib: unlink " // tail
end procedure unlink

module procedure exists
error stop "pathlib: %exists " // tail
end procedure exists

module procedure is_exe
error stop "pathlib: %is_exe " // tail
end procedure is_exe


module procedure cwd
error stop "pathlib: cwd() " // tail
end procedure cwd

module procedure is_symlink
error stop "pathlib: is_symlink() " // tail
end procedure is_symlink


module procedure size_bytes
error stop "pathlib: %size_bytes() " // tail
end procedure size_bytes

end submodule pathlib_dummy
