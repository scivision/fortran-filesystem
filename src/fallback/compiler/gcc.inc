submodule (filesystem) gcc_no_cpp_fs
!! GCC non-C++ filesystem

implicit none (type, external)

contains

module procedure f_unlink
intrinsic :: unlink
call unlink(path)
end procedure f_unlink


module procedure get_cwd

integer :: i
character(MAXP) :: work

i = getcwd(work)
if(i /= 0) error stop "filesystem:get_cwd: could not get current working dir"

get_cwd = as_posix(work)

end procedure get_cwd


module procedure is_dir

integer :: i, ftmode, statb(13)
character(:), allocatable :: wk

is_dir = .false.

wk = expanduser(path)
if(len_trim(wk) == 0) return

if(.not. sys_posix()) then
!! GCC Windows quirk workarounds
!! must not have trailing slash on Windows EXCEPT if root drive only
  wk = as_posix(wk)
  i = len_trim(wk)
  if (i > 3) then
    if(wk(i:i) == '/') wk = wk(1:i-1)
  elseif (i == 2) then
    if(wk(i:i) == ':') wk = wk // "/"  !< must have trailing slash if only root drive
  endif
endif

inquire(file=wk, exist=is_dir)
if(.not.is_dir) return

i = stat(wk, statb)
if(i /= 0) then
  is_dir = .false.
  return
endif

ftmode = iand(statb(3), O'0170000') !< file type mode

i = iand(ftmode, O'0040000')
is_dir = i == 16384

! print '(a,O8)', "TRACE:is_dir stat(3) file type octal: ", ftmode

end procedure is_dir


module procedure file_size

character(:), allocatable :: wk
integer :: ftmode, s(13), i

file_size = -1

wk = expanduser(path)
if(len_trim(wk) == 0) return

i = stat(wk, s)
if(i /= 0) then
  write(stderr,*) "file_size: could not stat file: ", wk
  return
endif

ftmode = iand(s(3), O'0170000') !< file type mode
! print '(a,O8)', "TRACE:file_size stat(3) file type octal: ", ftmode

if (iand(ftmode, O'0040000') == 16384) then
  write(stderr,*) "file_size: is a directory: ", wk
  return
endif

file_size = s(8)

end procedure file_size



end submodule gcc_no_cpp_fs
