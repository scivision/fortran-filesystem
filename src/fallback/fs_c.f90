submodule (filesystem) fs_c_int

implicit none (type,external)

interface
subroutine cfs_filesep(sep) bind(C, name='filesep')
import
character(kind=C_CHAR), intent(out) :: sep(*)
end subroutine

logical(C_BOOL) function cfs_is_absolute(path) bind(C, name='is_absolute')
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function cfs_is_dir(path) bind(C, name='is_dir')
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_root(path, result) bind(C, name="root")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: result(*)
end function


end interface

contains


module procedure is_dir
is_dir = cfs_is_dir(trim(path) // C_NULL_CHAR)
end procedure is_dir


module procedure filesep
character(kind=C_CHAR) :: cbuf(2)

call cfs_filesep(cbuf)
filesep = cbuf(1)

end procedure filesep


module procedure is_absolute
is_absolute = cfs_is_absolute(trim(path) // C_NULL_CHAR)
end procedure is_absolute


module procedure root
character(kind=c_char, len=3) :: cbuf
integer(C_SIZE_T) :: N

N = cfs_root(trim(path) // C_NULL_CHAR, cbuf)

root = trim(cbuf(:N))

end procedure root


end submodule fs_c_int
