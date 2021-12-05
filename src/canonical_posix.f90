submodule (pathlib) path_canon
!! This is for POSIX systems (MacOS, Linux, etc)
!! path need not exist.
!!
!! https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/realpath.3.html
!!
!! https://linux.die.net/man/3/realpath

use, intrinsic :: iso_c_binding, only: c_char, c_null_char

implicit none (type, external)

interface
subroutine realpath_c(path, rpath) bind(C, name="realpath")
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

if(len_trim(path) == 0) error stop "cannot canonicalize empty path"

wk = expanduser(path)

!! some systems e.g. Intel oneAPI Windows can't handle leading "." or ".."
!! so manually resolve this part with CWD, which is implicit.

if (wk(1:1) == ".") wk = cwd() // "/" // wk

if(len(wk) > N) error stop "path too long"

call realpath_c(wk // c_null_char, c_buf)

do i = 1,N
  if (c_buf(i) == c_null_char) exit
  buf(i:i) = c_buf(i)
enddo

canonical = trim(buf(:i-1))

end procedure canonical

end submodule path_canon
