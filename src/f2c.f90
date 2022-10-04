submodule (filesystem) fort2c_ifc

use, intrinsic :: iso_c_binding, only : C_INT, C_CHAR, C_NULL_CHAR, C_SIZE_T

implicit none

interface

integer(C_INT) function max_path() bind(C, name="fs_get_maxp")
import
end function

subroutine fs_as_posix(path) bind(C)
import
character(kind=C_CHAR), intent(inout) :: path(*)
end subroutine

subroutine fs_as_windows(path) bind(C)
import
character(kind=C_CHAR), intent(inout) :: path(*)
end subroutine

integer(C_SIZE_T) function fs_canonical(path, strict, result, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
logical(C_BOOL), intent(in), value :: strict
character(kind=C_CHAR), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

logical(C_BOOL) function fs_chmod_exe(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function fs_chmod_no_exe(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_INT) function fs_copy_file(source, dest, overwrite) bind(C)
import
character(kind=c_char), intent(in) :: source(*), dest(*)
logical(c_bool), intent(in), value :: overwrite
end function


logical(C_BOOL) function fs_equivalent(path1, path2) bind(C)
import C_BOOL, C_CHAR
character(kind=C_CHAR), intent(in) :: path1(*), path2(*)
end function

integer(C_SIZE_T) function fs_filesep(sep) bind(C)
import
character(kind=C_CHAR), intent(out) :: sep(*)
end function

logical(C_BOOL) function fs_is_symlink(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function fs_lib_dir(path, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(out) :: path(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_INT) function fs_create_symlink(target, link) bind(C)
import
character(kind=C_CHAR), intent(in) :: target(*), link(*)
end function

integer(C_INT) function fs_create_directories(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function fs_remove(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function fs_exists(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function fs_expanduser(path, result, buffer_size) bind(C)
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_file_name(path, filename, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: filename(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_file_size(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function fs_get_cwd(path, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(out) :: path(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_get_homedir(path, buffer_size) bind(C)
import
character(kind=c_char), intent(out) :: path(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_get_tempdir(path, buffer_size) bind(C)
import
character(kind=c_char), intent(out) :: path(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

logical(c_bool) function fs_is_absolute(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function fs_is_file(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function fs_is_dir(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(c_bool) function fs_is_exe(path) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

integer(C_SIZE_T) function fs_join(path, other, result, buffer_size) bind(C)
import
character(kind=c_char), intent(in) :: path(*), other(*)
character(kind=c_char), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_make_absolute(path, top_path, result, buffer_size) bind(C)
import
character(kind=c_char), intent(in) :: path(*), top_path(*)
character(kind=c_char), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_normal(path, result, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_parent(path, result, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_relative_to(path, base, result, buffer_size) bind(C)
import
character(kind=c_char), intent(in) :: path(*), base(*)
character(kind=c_char), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_root(path, result, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_stem(path, result, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

integer(C_SIZE_T) function fs_suffix(path, result, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*)
character(kind=C_CHAR), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

logical(c_bool) function fs_touch(path) bind(C)
import
character(kind=c_char), intent(in) :: path(*)
end function

integer(C_SIZE_T) function fs_with_suffix(path, new_suffix, result, buffer_size) bind(C)
import
character(kind=C_CHAR), intent(in) :: path(*), new_suffix
character(kind=C_CHAR), intent(out) :: result(*)
integer(C_SIZE_T), intent(in), value :: buffer_size
end function

end interface

contains

module procedure get_max_path
get_max_path = int(max_path())
end procedure

module procedure as_posix
character(kind=c_char, len=:), allocatable :: cbuf
allocate(character(len(path)+1) :: cbuf)
cbuf = trim(path) // C_NULL_CHAR
call fs_as_posix(cbuf)
r = cbuf(:len(path))
end procedure

module procedure as_windows
character(kind=c_char, len=:), allocatable :: cbuf
allocate(character(len(path)+1) :: cbuf)
cbuf = trim(path) // C_NULL_CHAR
call fs_as_windows(cbuf)
r = cbuf(:len(path))
end procedure

module procedure canonical
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
logical(c_bool) :: s
allocate(character(max_path()) :: cbuf)
s = .false.
if(present(strict)) s = strict
N = fs_canonical(trim(path) // C_NULL_CHAR, s, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: canonical)
canonical = cbuf(:N)
end procedure canonical

module procedure chmod_exe
logical :: s
s = fs_chmod_exe(trim(path) // C_NULL_CHAR)
if(present(ok)) ok = s
end procedure

module procedure chmod_no_exe
logical :: s
s = fs_chmod_no_exe(trim(path) // C_NULL_CHAR)
if(present(ok)) ok = s
end procedure


module procedure copy_file
logical(c_bool) :: ow
integer(C_INT) :: ierr
ow = .false.
if(present(overwrite)) ow = overwrite
ierr = fs_copy_file(trim(src) // C_NULL_CHAR, trim(dest) // C_NULL_CHAR, ow)
if (present(status)) then
  status = ierr
elseif(ierr /= 0) then
  error stop "failed to copy file: " // src // " to " // dest
endif
end procedure


module procedure mkdir
integer :: ierr

ierr = fs_create_directories(trim(path) // C_NULL_CHAR)
if(present(status)) then
  status = ierr
elseif (ierr /= 0) then
  write(stderr,'(a,i0)') "ERROR:filesystem:mkdir: failed to create directory: " // path // " error code: ", ierr
  error stop
endif
end procedure mkdir


module procedure create_symlink
integer(C_INT) :: ierr

ierr = fs_create_symlink(trim(tgt) // C_NULL_CHAR, trim(link) // C_NULL_CHAR)
if(present(status)) then
  status = ierr
elseif (ierr < 0) then
  error stop "ERROR:filesystem:create_symlink: platform is not capable of symlinks."
elseif (ierr /= 0) then
  error stop "ERROR:filesystem:create_symlink: " // link
endif
end procedure


module procedure exists
exists = fs_exists(trim(path) // C_NULL_CHAR)
end procedure

module procedure expanduser
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_expanduser(trim(path) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: expanduser)
expanduser = cbuf(:N)
end procedure

module procedure filesep
character(kind=C_CHAR) :: cbuf(2)
integer(C_SIZE_T) :: L
L = fs_filesep(cbuf)
if (L /= 1) error stop "ERROR:filesystem:filesep: failed to get file separator"
filesep = cbuf(1)
end procedure

module procedure file_name
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_file_name(trim(path) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: file_name)
file_name = cbuf(:N)
end procedure

module procedure file_size
file_size = fs_file_size(trim(path) // C_NULL_CHAR)
end procedure

module procedure get_cwd
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_get_cwd(cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: get_cwd)
get_cwd = cbuf(:N)
end procedure

module procedure get_homedir
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_get_homedir(cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: get_homedir)
get_homedir = cbuf(:N)
end procedure

module procedure get_tempdir
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_get_tempdir(cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: get_tempdir)
get_tempdir = cbuf(:N)
end procedure

module procedure is_absolute
!! no expanduser to be consistent with Python filesystem etc.
is_absolute = fs_is_absolute(trim(path) // C_NULL_CHAR)
end procedure

module procedure is_dir
is_dir = fs_is_dir(trim(path) // C_NULL_CHAR)
end procedure

module procedure is_exe
is_exe = fs_is_exe(trim(path) // C_NULL_CHAR)
end procedure

module procedure is_file
is_file = fs_is_file(trim(path) // C_NULL_CHAR)
end procedure

module procedure is_symlink
is_symlink = fs_is_symlink(trim(path) // C_NULL_CHAR)
end procedure

module procedure join
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_join(trim(path) // C_NULL_CHAR, trim(other) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: join)
join = cbuf(:N)
end procedure

module procedure lib_dir
character(kind=C_CHAR, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_lib_dir(cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: r)
r = cbuf(:N)
end procedure

module procedure make_absolute
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_make_absolute(trim(path) // C_NULL_CHAR, trim(top_path) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: make_absolute)
make_absolute = cbuf(:N)
end procedure

module procedure parent
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_parent(trim(path) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: r)
r = cbuf(:N)
end procedure

module procedure normal
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_normal(trim(path) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: normal)
normal = cbuf(:N)
end procedure

module procedure relative_to
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_relative_to(trim(a) // C_NULL_CHAR, trim(b) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: relative_to)
relative_to = cbuf(:N)
end procedure

module procedure remove
logical(c_bool) :: e
e = fs_remove(trim(path) // C_NULL_CHAR)
if (.not. e) write(stderr, '(a)') "filesystem:unlink: " // path // " may not have been deleted."
end procedure

module procedure root
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_root(trim(path) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: root)
root = cbuf(:N)
end procedure

module procedure same_file
same_file = fs_equivalent(trim(path1) // C_NULL_CHAR, trim(path2) // C_NULL_CHAR)
end procedure

module procedure stem
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_stem(trim(path) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: stem)
stem = cbuf(:N)
end procedure

module procedure suffix
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_suffix(trim(path) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: suffix)
suffix = cbuf(:N)
end procedure

module procedure touch
if(.not. fs_touch(trim(path) // C_NULL_CHAR)) error stop "filesystem:touch: " // path
end procedure

module procedure with_suffix
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = fs_with_suffix(trim(path) // C_NULL_CHAR, trim(new) // C_NULL_CHAR, cbuf, len(cbuf, kind=C_SIZE_T))
allocate(character(N) :: with_suffix)
with_suffix = cbuf(:N)
end procedure with_suffix

end submodule fort2c_ifc
