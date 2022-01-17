submodule (pathlib) pathlib_windows

use, intrinsic :: iso_c_binding, only: c_int, c_long, c_char, c_null_char

implicit none (type, external)

interface
integer(c_int) function mkdir_c(path) bind (C, name='_mkdir')
!! https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mkdir-wmkdir
import c_int, c_char
character(kind=c_char), intent(in) :: path(*)
end function mkdir_c

subroutine fullpath_c(absPath, relPath, maxLength) bind(c, name='_fullpath')
!! char *_fullpath(char *absPath, const char *relPath, size_t maxLength)
!! https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fullpath-wfullpath?view=vs-2019
import c_char, c_long
character(kind=c_char), intent(in) :: relPath(*)
character(kind=c_char), intent(out) :: absPath(*)
integer(c_long), intent(in) :: maxLength
end subroutine fullpath_c

end interface


contains


module procedure canonical

character(kind=c_char, len=:), allocatable :: wk
integer(c_long), parameter :: N = 4096
character(kind=c_char):: c_buf(N)
character(N) :: buf
integer :: i

if(len_trim(path) == 0) error stop "pathlib:canonical: cannot canonicalize empty path"

wk = expanduser(path)

!! some systems e.g. old MacOS can't handle leading "." or ".."
!! so manually resolve this part with CWD, which is implicit.

if (wk(1:1) == ".") wk = cwd() // "/" // wk

if(len(wk) > N) error stop "pathlib:canonical: path too long: " // wk

call fullpath_c(c_buf, wk // c_null_char, N)

do i = 1,N
  if (c_buf(i) == c_null_char) exit
  buf(i:i) = c_buf(i)
enddo

canonical = trim(buf(:i-1))

end procedure canonical


module procedure mkdir
!! https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mkdir-wmkdir
character(kind=c_char, len=:), allocatable :: wk
!! must use allocatable buffer, not direct substring to C

integer :: i
integer(c_int) :: ierr
character(:), allocatable :: pts(:)

wk = expanduser(path) !< not canonical
if (len_trim(wk) == 0) error stop 'pathlib:mkdir: must specify directory to create'

!! some systems can't handle leading "." or ".."
!! so manually resolve this part with CWD, which is implicit.
if (wk(1:1) == ".") wk = cwd() // "/" // wk

if(is_dir(wk)) return

call file_parts(wk, fparts=pts)

wk = trim(pts(1))
if(.not.is_dir(wk)) then
  ierr = mkdir_c(wk // C_NULL_CHAR)
  if (ierr /= 0) error stop 'pathlib:mkdir: could not create directory: ' // wk // ' from: ' // path
endif
do i = 2,size(pts)
  wk = trim(wk) // "/" // pts(i)
  if (is_dir(wk)) cycle

  ierr = mkdir_c(wk // C_NULL_CHAR)
  if (ierr /= 0) error stop 'pathlib:mkdir: could not create directory: ' // wk // ' from: ' // path
end do

end procedure mkdir


end submodule pathlib_windows
