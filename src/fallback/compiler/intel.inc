submodule (filesystem) intel_no_cpp_fs
!! Intel non-C++ filesystem

implicit none (type, external)

contains

module procedure f_unlink
use ifport, only : unlink
integer :: ierr
ierr = unlink(path)
if (ierr /= 0) error stop "filesystem:unlink failed: " // path
end procedure f_unlink


module procedure get_cwd
use ifport, only : getcwd

integer :: i
character(MAXP) :: work

i = getcwd(work)
if(i /= 0) error stop "filesystem:get_cwd: could not get current working dir"

get_cwd = as_posix(work)

end procedure get_cwd


end submodule intel_no_cpp_fs
