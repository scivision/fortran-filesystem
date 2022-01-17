!! these functions could be implemented via C runtime library,
!! but for speed/ease of implementation, for now we use
!! compiler-specific intrinsic functions

submodule (pathlib) pathlib_gcc

implicit none (type, external)

contains

module procedure cwd

integer :: i
character(4096) :: work

i = getcwd(work)
if(i /= 0) error stop "pathlib:cwd: could not get CWD"

cwd = as_posix(work)

end procedure cwd


module procedure size_bytes

character(:), allocatable :: wk
integer :: ftmode, s(13), i

size_bytes = -1

wk = expanduser(path)
if(len_trim(wk) == 0) return

i = stat(wk, s)
if(i /= 0) then
  write(stderr,*) "size_bytes: could not stat file: ", wk
  return
endif

ftmode = iand(s(3), O'0170000') !< file type mode
! print '(a,O8)', "TRACE:size_bytes stat(3) file type octal: ", ftmode

if (iand(ftmode, O'0040000') == 16384) then
  write(stderr,*) "size_bytes: is a directory: ", wk
  return
endif

size_bytes = s(8)

end procedure size_bytes


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
