!! these functions could be implemented via C runtime library,
!! but for speed/ease of implementation, for now we use
!! compiler-specific intrinsic functions
submodule (pathlib) pathlib_intel

implicit none (type, external)

contains

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


end submodule pathlib_intel
