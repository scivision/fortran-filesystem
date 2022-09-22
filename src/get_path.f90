submodule (filesystem) get_path_smod

use, intrinsic :: iso_c_binding, only: C_CHAR, C_SIZE_T

implicit none

interface !< get_path.c

integer(C_SIZE_T) function max_path() bind(C, name="get_maxp")
import
end function

integer(C_SIZE_T) function cfs_lib_path(path) bind(C, name="lib_path")
import
character(kind=C_CHAR), intent(out) :: path(*)
end function

integer(C_SIZE_T) function cfs_exe_path(path) bind(C, name="exe_path")
import
character(kind=C_CHAR), intent(out) :: path(*)
end function

end interface

contains


module procedure exe_path
character(kind=C_CHAR, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_exe_path(cbuf)
allocate(character(N) :: exe_path)
exe_path = cbuf(:N)
end procedure

module procedure lib_path
character(kind=C_CHAR, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_lib_path(cbuf)
allocate(character(N) :: lib_path)
lib_path = cbuf(:N)
end procedure

end submodule get_path_smod
