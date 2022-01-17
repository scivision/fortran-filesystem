submodule (pathlib) posix_crt

use, intrinsic :: iso_c_binding, only: c_int, c_char, c_null_char

implicit none (type, external)

interface

integer(c_int) function mkdir_c(path, mask) bind(C, name='mkdir')
!! https://linux.die.net/man/3/mkdir
import c_int, c_char
character(kind=c_char), intent(in) :: path(*)
integer(c_int), value, intent(in) :: mask
end function mkdir_c

subroutine realpath_c(path, rpath) bind(C, name="realpath")
!! https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/realpath.3.html
!! https://linux.die.net/man/3/realpath
import c_char
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: rpath(*)
end subroutine realpath_c

end interface

contains


module procedure canonical

character(kind=c_char, len=:), allocatable :: wk
integer, parameter :: N = 4096
character(kind=c_char):: c_buf(N)
character(N) :: buf
integer :: i

if(len_trim(path) == 0) error stop "pathlib:canonical: cannot canonicalize empty path"

wk = expanduser(path)

!! some systems can't handle leading "." or ".."
!! so manually resolve this part with CWD, which is implicit.

if (wk(1:1) == ".") wk = cwd() // "/" // wk

if(len(wk) > N) error stop "pathlib:canonical: path too long: " // wk

call realpath_c(wk // C_NULL_CHAR, c_buf)

do i = 1,N
  if (c_buf(i) == C_NULL_CHAR) exit
  buf(i:i) = c_buf(i)
enddo

canonical = trim(buf(:i-1))

end procedure canonical


module procedure mkdir
!! create a directory, with parents if needed

character(kind=c_char, len=:), allocatable :: wk
!! must use allocatable buffer, not direct substring to C

integer :: i
integer(c_int) :: ierr
character(:), allocatable :: pts(:)

wk = expanduser(path)  !< not canonical as it trims path part we want to create with mkdir
if (len_trim(wk) < 1) error stop 'pathlib:mkdir: must specify directory to create'

!! some systems can't handle leading "." or ".."
!! so manually resolve this part with CWD, which is implicit.
if (wk(1:1) == ".") wk = cwd() // "/" // wk

if(is_dir(wk)) return

call file_parts(wk, fparts=pts)

! print *, "TRACE: mkdir: ", wk, pts

wk = trim(pts(1))
if(.not.is_dir(wk)) then
  ierr = mkdir_c(wk // C_NULL_CHAR, int(o'755', c_int))
  if (ierr /= 0) error stop 'pathlib:mkdir: could not create directory: ' // wk // ' from: ' // path
endif
do i = 2,size(pts)
  wk = trim(wk) // "/" // trim(pts(i))
  if (is_dir(wk)) cycle

  ierr = mkdir_c(wk // C_NULL_CHAR, int(o'755', c_int))
  if (ierr /= 0) error stop 'pathlib:mkdir: could not create directory: ' // wk // ' from: ' // path
end do

end procedure mkdir


end submodule posix_crt
