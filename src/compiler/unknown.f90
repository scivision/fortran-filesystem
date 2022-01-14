submodule (pathlib) pathlib_dummy
!! generic routine for non-Intel, non-GCC.
!! better to make custom per-compiler routine based on pathlib_gcc for other compilers.

implicit none (type, external)

contains


module procedure is_dir
inquire(file=expanduser(path), exist=is_dir)
end procedure is_dir


module procedure is_exe
error stop "pathlib: %is_exe method not supported on this compiler yet. Please open GitHub Issue request."
end procedure is_exe


module procedure cwd
error stop "pathlib: cwd() not supported on this compiler yet. Please open GitHub Issue request."
end procedure cwd


module procedure size_bytes
error stop "pathlib: %size_bytes not yet supported on this compiler."
end procedure size_bytes

end submodule pathlib_dummy
