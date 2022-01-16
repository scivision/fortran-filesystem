submodule (pathlib) fs_cpp

use, intrinsic :: iso_c_binding, only : c_bool, c_char

implicit none (type, external)

interface !< fs.cpp
logical(c_bool) function fs_is_symlink(path) bind(C, name="is_symlink")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function fs_is_symlink

end interface

contains


module procedure is_symlink
character(kind=c_char, len=:), allocatable :: cpath

cpath = expanduser(path)
is_symlink = fs_is_symlink(cpath)
end procedure is_symlink


end submodule fs_cpp
