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

end interface

contains

module procedure filesep
character(kind=C_CHAR) :: cbuf(2)

call cfs_filesep(cbuf)
filesep = cbuf(1)

end procedure filesep


module procedure is_absolute
is_absolute = cfs_is_absolute(trim(path) // C_NULL_CHAR)
end procedure is_absolute


end submodule fs_c_int
