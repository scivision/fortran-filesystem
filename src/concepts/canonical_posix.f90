module canonical
!! This is for POSIX systems (MacOS, Linux, etc)
!! path need not exist.
!!
!! https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/realpath.3.html
!!
!! https://linux.die.net/man/3/realpath

use, intrinsic :: iso_c_binding, only: c_char, c_null_char
implicit none (type, external)
public :: realpath

interface
subroutine realpath_c(path, rpath) bind(c, name='realpath')
import c_char
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: rpath(*)
end subroutine realpath_c
end interface

contains

impure function realpath(path)

character(:), allocatable :: realpath
character(*), intent(in) :: path

integer, parameter :: N = 4096
character(kind=c_char):: c_buf(N)
character(N) :: buf
integer :: i

if(len_trim(path) == 0) error stop "cannot canonicalize empty path"
if(len(path) > N) error stop "path too long"

call realpath_c(path // c_null_char, c_buf)

do i = 1,N
  if (c_buf(i) == c_null_char) exit
  buf(i:i) = c_buf(i)
enddo

realpath = trim(buf(:i-1))

end function realpath

end module canonical
