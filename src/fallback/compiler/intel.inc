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


module procedure file_size
use ifport, only : stat

character(:), allocatable :: wk
integer :: s(12), i

file_size = -1
wk = expanduser(path)

i = stat(wk, s)
if(i /= 0) then
  write(stderr,*) "file_size: could not stat file: ", wk
  return
endif

if (iand(s(3), O'0040000') == 16384) then
  write(stderr,*) "file_size: is a directory: ", wk
  return
endif

file_size = s(8)

end procedure file_size


end submodule intel_no_cpp_fs
