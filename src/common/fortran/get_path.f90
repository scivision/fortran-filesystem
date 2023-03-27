submodule (filesystem) get_path_smod

use, intrinsic :: iso_c_binding, only: C_CHAR, C_SIZE_T

implicit none

interface !< get_path.c

integer(C_SIZE_T) function max_path() bind(C, name="fs_get_max_path")
import
end function

integer(C_SIZE_T) function fs_lib_path(path, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(out) :: path(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_exe_path(path, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(out) :: path(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

end interface

contains


module procedure exe_path
character(kind=C_CHAR, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_exe_path(cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: exe_path)
exe_path = cbuf(:N)
end procedure

module procedure lib_path
character(kind=C_CHAR, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_lib_path(cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: lib_path)
lib_path = cbuf(:N)
end procedure

end submodule get_path_smod
