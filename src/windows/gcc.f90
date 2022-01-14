submodule (pathlib) windows_gcc
!! functions for GCC on Windows (non-POSIX) systems

implicit none (type, external)

contains

module procedure is_symlink
!! TODO: Windows MinGW GCC does not detect symlinks even with LSTAT().
!! Investigate Windows:
!! * name surrogate reparse point IO_REPARSE_TAG_SYMLINK
!! * directory junction IO_REPARSE_TAG_MOUNT_POINT
!! C stdlib lstat() does not exist on Windows even with MinGW
!! C++ Boost and Python os.stat have implemented symlinks on Windows.

is_symlink = .false.

end procedure is_symlink

end submodule windows_gcc
