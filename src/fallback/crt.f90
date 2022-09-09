submodule (filesystem) posix_crt

use, intrinsic :: iso_c_binding, only: C_INT

implicit none

interface
integer(C_INT) function fs_realpath(path, rpath) bind(C)
!! https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/realpath.3.html
!! https://linux.die.net/man/3/realpath
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: rpath(*)
end function
end interface

contains


module procedure canonical

character(kind=C_CHAR, len=:), allocatable :: wk
character(kind=C_CHAR), allocatable :: c_buf(:)
character(:), allocatable :: buf
integer :: i, L, N

L = get_max_path()

allocate(character(L) :: buf)
allocate(c_buf(L))

if(len_trim(path) == 0) then
  canonical = ""
  return
endif

wk = trim(expanduser(path))

!! some systems can't handle leading "." or ".."

if (wk(1:1) == ".") wk = trim(get_cwd()) // "/" // wk

if(len(wk) > L) error stop "filesystem:canonical: path too long: " // wk

N = fs_realpath(wk // C_NULL_CHAR, c_buf)

do i = 1, N
  if (c_buf(i) == C_NULL_CHAR) exit
  buf(i:i) = c_buf(i)
enddo

canonical = as_posix(buf(:i-1))

end procedure canonical


end submodule posix_crt
