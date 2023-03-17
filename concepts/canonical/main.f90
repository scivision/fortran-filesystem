!! Reference discussion (excellent) https://stackoverflow.com/a/34271901/7703794

program my_dir
!! get executable's directory, regardless of PWD or CWD

use, intrinsic :: iso_c_binding, only : C_CHAR, C_INT, C_NULL_CHAR

implicit none

interface
integer(C_INT) function fs_realpath(path, rpath) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: rpath(*)
end function
end interface

character(:), allocatable :: exe_dir, resolved
character(4096, kind=C_CHAR) :: buf

integer :: L, ierr

call get_command_argument(0, buf, length=L, status=ierr)
if(ierr /= 0) error stop "ERROR: get_command_argument(0) failed"
if(L < 2) error stop "ERROR: get_command_argument(0) returned L < 2: " // trim(buf)

!! gfortran (Windows): full path
!! gfortran (Linux): relative path
!! ifort/ifx (Windows or Linux), nvfortran, flang: relative path

resolved = canonical(buf)
print '(A)', "canonical(argv[0]) = " // trim(resolved)

contains

function canonical(path)

character(*), intent(in) :: path
character(:), allocatable :: canonical

character(kind=C_CHAR, len=:), allocatable :: cbuf
integer :: N

integer, parameter :: MAX = 4096
!! arbitrary PATH_MAX, see Ffilesystem for robust implementation

allocate(character(MAX) :: cbuf)
N = fs_realpath(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: canonical)
canonical = cbuf(:N)

end function canonical

end program
