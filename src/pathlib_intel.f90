submodule (pathlib) pathlib_intel

implicit none (type, external)

contains

module procedure is_directory

type(path_t) :: p

p = self%expanduser()

inquire(directory=p%path, exist=is_directory)

end procedure is_directory


module procedure executable
use ifport, only : stat

integer :: s(12), ierr, iu, ig

executable = .false.

if (.not. self%is_file()) return

ierr = stat(self%path, s)
if(ierr /= 0) return

iu = iand(s(3), O'0000100')
ig = iand(s(3), O'0000010')
executable = (iu == 64 .or. ig == 8)

end procedure executable

end submodule pathlib_intel
