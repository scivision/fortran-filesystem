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
if(i /= 0) error stop "could not get CWD"

cwd = trim(work)

end procedure cwd


module procedure is_dir

integer :: i, statb(13)
character(:), allocatable :: wk

wk = expanduser(path)

!! must not have trailing slash on Windows
i = len_trim(wk)
if (wk(i:i) == '/') wk = wk(1:i-1)

inquire(file=wk, exist=is_dir)
if(.not.is_dir) return

i = stat(wk, statb)
if(i /= 0) then
  is_dir = .false.
  return
endif

i = iand(statb(3), O'0040000')
is_dir = i == 16384

! print '(O8)', statb(3)

end procedure is_dir


module procedure size_bytes

character(:), allocatable :: wk
integer :: s(13), i

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

character(:), allocatable :: wk
integer :: s(13), iu, ig, i

executable = .false.
wk = expanduser(self%path_str)

i = stat(wk, s)
if(i /= 0) then
  write(stderr,*) "executable: could not stat file: ", wk
  return
endif

if (iand(s(3), O'0040000') == 16384) then
  write(stderr,*) "executable: is a directory: ", wk
  return
endif

iu = iand(s(3), O'0000100')
ig = iand(s(3), O'0000010')
executable = (iu == 64 .or. ig == 8)

end procedure executable


end submodule pathlib_gcc
