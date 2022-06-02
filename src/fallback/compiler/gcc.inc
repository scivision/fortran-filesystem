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
