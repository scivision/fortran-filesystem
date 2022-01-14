submodule (pathlib) windows_intel
!! functions for Intel compiler on Windows (non-POSIX) systems

implicit none (type, external)

contains

module procedure is_symlink
!! TODO: Windows Intel oneAPI does not detect symlinks even with LSTAT().
!! see comments in windows/gcc.f90

is_symlink = .false.

end procedure is_symlink

end submodule windows_intel
