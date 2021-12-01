submodule (pathlib) pathlib_dummy
!! generic routine for non-Intel, non-GCC.
!! better to make custom per-compiler routine based on pathlib_gcc for other compilers.

implicit none (type, external)

contains

module procedure is_directory

inquire(file=expanduser(path), exist=exists)

end procedure is_directory

end submodule pathlib_dummy
