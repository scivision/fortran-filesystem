submodule (pathlib) intel_no_cpp_fs
!! Intel non-C++17 filesystem

implicit none (type, external)

contains

module procedure f_unlink
use ifport, only : unlink
call unlink(path)
end procedure f_unlink


module procedure is_dir
inquire(directory=expanduser(path), exist=is_dir)
end procedure is_dir



end submodule intel_no_cpp_fs
