!! these functions could be implemented via C runtime library,
!! but for speed/ease of implementation, for now we use
!! compiler-specific intrinsic functions
submodule (pathlib) pathlib_intel

implicit none (type, external)

contains

module procedure cwd
use ifport, only : getcwd

integer :: i
character(4096) :: work

i = getcwd(work)
if(i /= 0) error stop "could not get CWD"

cwd = trim(work)

end procedure cwd


module procedure is_dir
inquire(directory=expanduser(path), exist=is_dir)
end procedure is_dir


module procedure size_bytes
use ifport, only : stat

character(:), allocatable :: wk
integer :: s(12), i

size_bytes = 0
wk = expanduser(path)

i = stat(wk, s)
if(i /= 0) then
  write(stderr,*) "size_bytes: could not stat file: ", wk
  return
endif

if (iand(s(3), O'0040000') == 16384) then
  write(stderr,*) "size_bytes: is a directory: ", wk
  return
endif

size_bytes = s(8)

end procedure size_bytes


module procedure executable
use ifport, only : stat

type(path_t) :: wk
integer :: s(12), ierr, iu, ig

executable = .false.

wk = self%expanduser()
if (.not. wk%is_file()) return

ierr = stat(wk%path_str, s)
if(ierr /= 0) return

iu = iand(s(3), O'0000100')
ig = iand(s(3), O'0000010')
executable = (iu == 64 .or. ig == 8)

end procedure executable


end submodule pathlib_intel
