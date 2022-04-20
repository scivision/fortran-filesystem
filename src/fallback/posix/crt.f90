submodule (filesystem) posix_crt

use, intrinsic :: iso_c_binding, only: c_int, c_char, c_null_char

implicit none (type, external)

interface

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
character(kind=c_char):: c_buf(MAXP)
character(MAXP) :: buf
integer :: i

if(len_trim(path) == 0) then
  canonical = ""
  return
endif

wk = expanduser(path)

!! some systems can't handle leading "." or ".."

if (wk(1:1) == ".") wk = get_cwd() // "/" // wk

if(len(wk) > MAXP) error stop "filesystem:canonical: path too long: " // wk

call realpath_c(wk // C_NULL_CHAR, c_buf)

do i = 1, MAXP
  if (c_buf(i) == C_NULL_CHAR) exit
  buf(i:i) = c_buf(i)
enddo

canonical = trim(buf(:i-1))

end procedure canonical


end submodule posix_crt
