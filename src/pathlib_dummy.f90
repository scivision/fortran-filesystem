submodule (pathlib) pathlib_dummy
!! generic routine for non-Intel, non-GCC.
!! better to make custom per-compiler routine based on pathlib_gcc for other compilers.

implicit none (type, external)

contains

module procedure is_directory

type(path_t) :: p

p = self%expanduser()

inquire(file=p%path_str, exist=is_directory)

end procedure is_directory


module procedure executable

error stop "%executable method not supported on this compiler yet. Please open GitHub Issue request."

end procedure executable


module procedure cwd

error stop "cwd() not supported on this compiler yet. Please open GitHub Issue request."

end procedure cwd

end submodule pathlib_dummy
