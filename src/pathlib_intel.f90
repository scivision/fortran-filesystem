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

module procedure is_directory

type(path_t) :: p

p = self%expanduser()

inquire(directory=p%path_str, exist=is_directory)

end procedure is_directory


module procedure size_bytes
use ifport, only : stat

type(path_t) :: wk
integer :: s(12), i

size_bytes = 0

wk = self%expanduser()
if(.not. wk%is_file()) return

i = stat(wk%path_str, s)
if(i /= 0) return

size_bytes = s(8)

end procedure size_bytes


module procedure executable
use ifport, only : stat

integer :: s(12), ierr, iu, ig

executable = .false.

if (.not. self%is_file()) return

ierr = stat(self%path_str, s)
if(ierr /= 0) return

iu = iand(s(3), O'0000100')
ig = iand(s(3), O'0000010')
executable = (iu == 64 .or. ig == 8)

end procedure executable


end submodule pathlib_intel
