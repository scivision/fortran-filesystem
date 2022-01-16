submodule (pathlib) fs_cpp

use, intrinsic :: iso_c_binding, only : c_bool, c_char, C_NULL_CHAR

implicit none (type, external)

interface !< fs.cpp
logical(c_bool) function fs_is_symlink(path) bind(C, name="is_symlink")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function fs_is_symlink

subroutine fs_create_directory_symlink(target, link) bind(C, name="create_directory_symlink")
import c_char
character(kind=c_char), intent(in) :: target(*), link(*)
end subroutine fs_create_directory_symlink

subroutine fs_create_symlink(target, link) bind(C, name="create_symlink")
import c_char
character(kind=c_char), intent(in) :: target(*), link(*)
end subroutine fs_create_symlink

logical(c_bool) function fs_remove(path) bind(C, name="remove")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function fs_remove

end interface

contains


module procedure is_symlink
character(kind=c_char, len=:), allocatable :: cpath

cpath = expanduser(path) // C_NULL_CHAR
is_symlink = fs_is_symlink(cpath)
end procedure is_symlink


module procedure create_symlink
character(kind=c_char, len=:), allocatable :: ctgt, clink

ctgt = expanduser(tgt) // C_NULL_CHAR
clink = expanduser(link) // C_NULL_CHAR

if (is_dir(tgt)) then
  call fs_create_directory_symlink(ctgt, clink)
else
  call fs_create_symlink(ctgt, clink)
endif

end procedure create_symlink


module procedure unlink

character(kind=c_char, len=:), allocatable :: cpath

logical :: e

cpath = expanduser(path) // C_NULL_CHAR
e = fs_remove(cpath)

end procedure unlink


end submodule fs_cpp
