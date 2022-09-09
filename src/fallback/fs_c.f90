submodule (filesystem) fs_c_int

use, intrinsic :: iso_c_binding, only : C_INT

implicit none

interface

integer(C_INT) function max_path() bind(C, name="get_maxp")
import C_INT
end function
subroutine cfs_filesep(sep) bind(C, name='filesep')
import
character(kind=C_CHAR), intent(out) :: sep(*)
end subroutine

logical(c_bool) function cfs_is_exe(path) bind(C, name="is_exe")
import
character(kind=c_char), intent(in) :: path(*)
end function

logical(c_bool) function cfs_is_symlink(path) bind(C, name="is_symlink")
import
character(kind=c_char), intent(in) :: path(*)
end function

integer(C_INT) function cfs_create_symlink(target, link) bind(C, name="create_symlink")
import
character(kind=c_char), intent(in) :: target(*), link(*)
end function

integer(C_SIZE_T) function cfs_file_size(path) bind(C, name="file_size")
import
character(kind=c_char), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_get_cwd(path) bind(C, name="get_cwd")
import
character(kind=c_char), intent(out) :: path(*)
end function

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

logical(c_bool) function cfs_chmod_exe(path) bind(C, name="chmod_exe")
import
character(kind=c_char), intent(in) :: path(*)
end function

logical(c_bool) function cfs_chmod_no_exe(path) bind(C, name="chmod_no_exe")
import
character(kind=c_char), intent(in) :: path(*)
end function

end interface


contains

module procedure get_max_path
get_max_path = int(max_path())
end procedure


module procedure is_dir
is_dir = cfs_is_dir(trim(path) // C_NULL_CHAR)
end procedure is_dir


module procedure is_exe
is_exe = cfs_is_exe(trim(path) // C_NULL_CHAR)
end procedure is_exe


module procedure filesep
character(kind=C_CHAR) :: cbuf(2)

call cfs_filesep(cbuf)
filesep = cbuf(1)

end procedure filesep


module procedure file_size
file_size = cfs_file_size(trim(path) // C_NULL_CHAR)
end procedure file_size


module procedure get_cwd
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_get_cwd(cbuf)

allocate(character(N) :: get_cwd)
get_cwd = as_posix(cbuf(:N))

end procedure get_cwd


module procedure is_absolute
is_absolute = cfs_is_absolute(trim(path) // C_NULL_CHAR)
end procedure is_absolute


module procedure is_symlink
is_symlink = cfs_is_symlink(trim(path) // C_NULL_CHAR)
end procedure is_symlink


module procedure create_symlink
integer(C_INT) :: ierr

ierr = cfs_create_symlink(trim(tgt) // C_NULL_CHAR, trim(link) // C_NULL_CHAR)
if(present(status)) then
  status = ierr
elseif (ierr < 0) then
  error stop "ERROR:filesystem:create_symlink: platform is not capable of symlinks."
elseif (ierr /= 0) then
  error stop "ERROR:filesystem:create_symlink: " // link
endif
end procedure create_symlink


module procedure root
character(kind=c_char, len=3) :: cbuf
integer(C_SIZE_T) :: N

N = cfs_root(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: root)
root = cbuf(:N)

end procedure root

module procedure chmod_exe
logical :: s

s = cfs_chmod_exe(trim(path) // C_NULL_CHAR)
if(present(ok)) ok = s
end procedure chmod_exe


module procedure chmod_no_exe
logical :: s

s = cfs_chmod_no_exe(trim(path) // C_NULL_CHAR)
if(present(ok)) ok = s
end procedure chmod_no_exe


end submodule fs_c_int
