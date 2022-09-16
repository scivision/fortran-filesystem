!! Reference: https://github.com/gpakosz/whereami

program my_dir
!! get module (shared library) directory, regardless of PWD or CWD

use, intrinsic :: iso_c_binding, only : C_CHAR, C_NULL_CHAR, C_SIZE_T

implicit none

interface
integer(C_SIZE_T) function get_libpath(path) bind(C)
import
character(kind=C_CHAR), intent(out) :: path(*)
end function
end interface

character(:), allocatable :: libpath
character(4096, kind=c_char) :: buf

integer :: L

L = get_libpath(buf)

allocate(character(len=L) :: libpath)
libpath = buf(:L)

print *, libpath


end program
