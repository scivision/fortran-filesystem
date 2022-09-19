submodule (filesystem) fort2c_ifc

use, intrinsic :: iso_c_binding, only : C_INT, C_CHAR, C_NULL_CHAR, C_SIZE_T

implicit none

interface

integer(C_INT) function max_path() bind(C, name="get_maxp")
import
end function

logical(C_BOOL) function cfs_chmod_exe(path) bind(C, name="chmod_exe")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function cfs_chmod_no_exe(path) bind(C, name="chmod_no_exe")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

subroutine cfs_filesep(sep) bind(C, name='filesep')
import
character(kind=C_CHAR), intent(out) :: sep(*)
end subroutine

logical(C_BOOL) function cfs_is_symlink(path) bind(C, name="is_symlink")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_INT) function cfs_create_symlink(target, link) bind(C, name="create_symlink")
import
character(kind=C_CHAR), intent(in) :: target(*), link(*)
end function

logical(C_BOOL) function cfs_remove(path) bind(C, name="fs_remove")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function cfs_exists(path) bind(C, name="exists")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_file_size(path) bind(C, name="file_size")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_get_cwd(path) bind(C, name="get_cwd")
import
character(kind=C_CHAR), intent(out) :: path(*)
end function

logical(c_bool) function cfs_is_absolute(path) bind(C, name="is_absolute")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function cfs_is_file(path) bind(C, name="is_file")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function cfs_is_dir(path) bind(C, name="is_dir")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(c_bool) function cfs_is_exe(path) bind(C, name="is_exe")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_normal(path, normalized) bind(C, name="normal")
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: normalized(*)
end function

integer(C_SIZE_T) function cfs_root(path, result) bind(C, name="root")
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: result(*)
end function

end interface

contains

module procedure get_max_path
get_max_path = int(max_path())
end procedure


module procedure chmod_exe
logical :: s
s = cfs_chmod_exe(trim(path) // C_NULL_CHAR)
if(present(ok)) ok = s
end procedure

module procedure chmod_no_exe
logical :: s
s = cfs_chmod_no_exe(trim(path) // C_NULL_CHAR)
if(present(ok)) ok = s
end procedure

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

module procedure exists
exists = cfs_exists(trim(path) // C_NULL_CHAR)
end procedure

module procedure filesep
character(kind=C_CHAR) :: cbuf(2)
call cfs_filesep(cbuf)
filesep = cbuf(1)
end procedure

module procedure file_size
file_size = cfs_file_size(trim(path) // C_NULL_CHAR)
end procedure

module procedure get_cwd
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_get_cwd(cbuf)

allocate(character(N) :: get_cwd)
get_cwd = cbuf(:N)

end procedure get_cwd

module procedure is_absolute
!! no expanduser to be consistent with Python filesystem etc.
is_absolute = cfs_is_absolute(trim(path) // C_NULL_CHAR)
end procedure

module procedure is_dir
is_dir = cfs_is_dir(trim(path) // C_NULL_CHAR)
end procedure

module procedure is_exe
is_exe = cfs_is_exe(trim(path) // C_NULL_CHAR)
end procedure

module procedure is_file
is_file = cfs_is_file(trim(path) // C_NULL_CHAR)
end procedure

module procedure is_symlink
is_symlink = cfs_is_symlink(trim(path) // C_NULL_CHAR)
end procedure

module procedure normal
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_normal(trim(path) // C_NULL_CHAR, cbuf)
allocate(character(N) :: normal)
normal = cbuf(:N)
end procedure normal

module procedure remove
logical(c_bool) :: e
e = cfs_remove(trim(path) // C_NULL_CHAR)
if (.not. e) write(stderr, '(a)') "filesystem:unlink: " // path // " may not have been deleted."
end procedure

module procedure root
character(kind=c_char, len=3) :: cbuf
integer(C_SIZE_T) :: N
N = cfs_root(trim(path) // C_NULL_CHAR, cbuf)
allocate(character(N) :: root)
root = cbuf(:N)
end procedure

end submodule fort2c_ifc
