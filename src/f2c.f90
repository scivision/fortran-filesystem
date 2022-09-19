submodule (filesystem) fort2c_ifc

use, intrinsic :: iso_c_binding, only : C_INT, C_CHAR, C_NULL_CHAR, C_SIZE_T

implicit none

interface

integer(C_INT) function max_path() bind(C, name="get_maxp")
import
end function

integer(C_SIZE_T) function cfs_canonical(path, strict, canonicalized) bind(C, name="canonical")
import
character(kind=C_CHAR), intent(in) :: path(*)
logical(C_BOOL), intent(in), value :: strict
character(kind=C_CHAR), intent(out) :: canonicalized(*)
end function

logical(C_BOOL) function cfs_chmod_exe(path) bind(C, name="chmod_exe")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function cfs_chmod_no_exe(path) bind(C, name="chmod_no_exe")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function


logical(C_BOOL) function cfs_equivalent(path1, path2) bind(C, name="equivalent")
import C_BOOL, C_CHAR
character(kind=C_CHAR), intent(in) :: path1(*), path2(*)
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

integer(C_SIZE_T) function cfs_expanduser(path, result) bind(C, name="expanduser")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: result(*)
end function

integer(C_SIZE_T) function cfs_file_name(path, filename) bind(C, name="file_name")
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: filename(*)
end function

integer(C_SIZE_T) function cfs_file_size(path) bind(C, name="file_size")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_get_cwd(path) bind(C, name="get_cwd")
import
character(kind=C_CHAR), intent(out) :: path(*)
end function

integer(C_SIZE_T) function cfs_get_homedir(path) bind(C, name="get_homedir")
import
character(kind=c_char), intent(out) :: path(*)
end function

integer(C_SIZE_T) function cfs_get_tempdir(path) bind(C, name="get_tempdir")
import
character(kind=c_char), intent(out) :: path(*)
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

integer(C_SIZE_T) function cfs_parent(path, fparent) bind(C, name="parent")
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: fparent(*)
end function

integer(C_SIZE_T) function cfs_root(path, result) bind(C, name="root")
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: result(*)
end function

integer(C_SIZE_T) function cfs_stem(path, fstem) bind(C, name="stem")
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: fstem(*)
end function

integer(C_SIZE_T) function cfs_suffix(path, fsuffix) bind(C, name="suffix")
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: fsuffix(*)
end function

end interface

contains

module procedure get_max_path
get_max_path = int(max_path())
end procedure


module procedure canonical
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
logical(c_bool) :: s
allocate(character(max_path()) :: cbuf)
s = .false.
if(present(strict)) s = strict
N = cfs_canonical(trim(path) // C_NULL_CHAR, s, cbuf)
allocate(character(N) :: canonical)
canonical = cbuf(:N)
end procedure canonical

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

module procedure expanduser
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_expanduser(trim(path) // C_NULL_CHAR, cbuf)
allocate(character(N) :: expanduser)
expanduser = cbuf(:N)
end procedure

module procedure filesep
character(kind=C_CHAR) :: cbuf(2)
call cfs_filesep(cbuf)
filesep = cbuf(1)
end procedure

module procedure file_name
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_file_name(trim(path) // C_NULL_CHAR, cbuf)
allocate(character(N) :: file_name)
file_name = cbuf(:N)
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
end procedure

module procedure get_homedir
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_get_homedir(cbuf)
allocate(character(N) :: get_homedir)
get_homedir = cbuf(:N)
end procedure

module procedure get_tempdir
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_get_tempdir(cbuf)
allocate(character(N) :: get_tempdir)
get_tempdir = cbuf(:N)
end procedure

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

module procedure parent
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_parent(trim(path) // C_NULL_CHAR, cbuf)
allocate(character(N) :: parent)
parent = cbuf(:N)
end procedure

module procedure normal
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_normal(trim(path) // C_NULL_CHAR, cbuf)
allocate(character(N) :: normal)
normal = cbuf(:N)
end procedure

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

module procedure same_file
same_file = cfs_equivalent(trim(path1) // C_NULL_CHAR, trim(path2) // C_NULL_CHAR)
end procedure

module procedure stem
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_stem(trim(path) // C_NULL_CHAR, cbuf)
allocate(character(N) :: stem)
stem = cbuf(:N)
end procedure

module procedure suffix
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_suffix(trim(path) // C_NULL_CHAR, cbuf)
allocate(character(N) :: suffix)
suffix = cbuf(:N)
end procedure

end submodule fort2c_ifc
