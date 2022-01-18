submodule (pathlib) intel_no_cpp_fs
!! Intel non-C++17 filesystem

implicit none (type, external)

contains

module procedure f_unlink
use ifport, only : unlink
call unlink(path)
end procedure f_unlink


module procedure get_cwd
use ifport, only : getcwd

integer :: i
character(4096) :: work

i = getcwd(work)
if(i /= 0) error stop "pathlib:get_cwd: could not get current working dir"

get_cwd = as_posix(work)

end procedure get_cwd


module procedure is_dir
inquire(directory=expanduser(path), exist=is_dir)
end procedure is_dir


module procedure is_exe
use ifport, only : stat

character(:), allocatable :: wk
integer :: s(12), i, iu, ig

is_exe = .false.
wk = expanduser(path)

i = stat(wk, s)
if(i /= 0) then
  write(stderr,*) "is_exe: could not stat file: ", wk
  return
endif

if (iand(s(3), O'0040000') == 16384) then
  write(stderr,*) "is_exe: is a directory: ", wk
  return
endif

iu = iand(s(3), O'0000100')
ig = iand(s(3), O'0000010')
is_exe = (iu == 64 .or. ig == 8)

end procedure is_exe


module procedure file_size

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
