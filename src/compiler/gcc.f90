!! these functions could be implemented via C runtime library,
!! but for speed/ease of implementation, for now we use
!! compiler-specific intrinsic functions

submodule (pathlib) pathlib_gcc

implicit none (type, external)

contains

module procedure is_exe

character(:), allocatable :: wk
integer :: s(13), ftmode, i

is_exe = .false.

wk = expanduser(path)
if(len_trim(wk) == 0) return

i = stat(wk, s)
if(i /= 0) then
  write(stderr,*) "is_exe: could not stat file: ", wk
  return
endif

ftmode = iand(s(3), O'0170000') !< file type mode

if (iand(ftmode, O'0040000') == 16384) then
  write(stderr,*) "is_exe: is a directory: ", wk
  return
endif

is_exe = (iand(s(3), O'0000100') == 64 .or. &
          iand(s(3), O'0000010') == 8)

end procedure is_exe


end submodule pathlib_gcc
