submodule (filesystem) fs_cpp

use, intrinsic :: iso_c_binding, only : c_char, c_ptr, C_INT, C_NULL_CHAR, C_SIZE_T

implicit none

interface !< fs.cpp

integer(C_INT) function max_path() bind(C, name="get_maxp")
import C_INT
end function

subroutine cfs_filesep(sep) bind(C, name='filesep')
import
character(kind=c_char), intent(out) :: sep(*)
end subroutine

logical(C_BOOL) function cfs_match(path, pattern) bind(C, name='match')
import
character(kind=c_char), intent(in) :: path, pattern
end function

integer(C_SIZE_T) function cfs_file_name(path, filename) bind(C, name="file_name")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: filename(*)
end function

integer(C_SIZE_T) function cfs_stem(path, fstem) bind(C, name="stem")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: fstem(*)
end function

integer(C_SIZE_T) function cfs_parent(path, fparent) bind(C, name="parent")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: fparent(*)
end function

integer(C_SIZE_T) function cfs_suffix(path, fsuffix) bind(C, name="suffix")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: fsuffix(*)
end function

integer(C_SIZE_T) function cfs_with_suffix(path, new_suffix, swapped) bind(C, name="with_suffix")
import
character(kind=c_char), intent(in) :: path(*), new_suffix
character(kind=c_char), intent(out) :: swapped(*)
end function


integer(C_SIZE_T) function cfs_normal(path, normalized) bind(C, name="normal")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: normalized(*)
end function

logical(c_bool) function cfs_is_symlink(path) bind(C, name="is_symlink")
import
character(kind=c_char), intent(in) :: path(*)
end function

integer(C_INT) function cfs_create_symlink(target, link) bind(C, name="create_symlink")
import
character(kind=c_char), intent(in) :: target(*), link(*)
end function

integer(C_INT) function cfs_create_directories(path) bind(C, name="create_directories")
import
character(kind=c_char), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_canonical(path, strict, canonicalized) bind(C, name="canonical")
import
character(kind=c_char), intent(in) :: path(*)
logical(c_bool), intent(in), value :: strict
character(kind=c_char), intent(out) :: canonicalized(*)
end function

logical(c_bool) function cfs_remove(path) bind(C, name="fs_remove")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function

logical(c_bool) function cfs_exists(path) bind(C, name="exists")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function

logical(c_bool) function cfs_is_file(path) bind(C, name="is_file")
import
character(kind=c_char), intent(in) :: path(*)
end function

logical(c_bool) function cfs_is_dir(path) bind(C, name="is_dir")
import
character(kind=c_char), intent(in) :: path(*)
end function

logical(c_bool) function cfs_equivalent(path1, path2) bind(C, name="equivalent")
import c_bool, c_char
character(kind=c_char), intent(in) :: path1(*), path2(*)
end function

integer(C_INT) function cfs_copy_file(source, dest, overwrite) bind(C, name="copy_file")
import
character(kind=c_char), intent(in) :: source(*), dest(*)
logical(c_bool), intent(in), value :: overwrite
end function

integer(C_SIZE_T) function cfs_relative_to(path, base, result) bind(C, name="relative_to")
import
character(kind=c_char), intent(in) :: path(*), base(*)
character(kind=c_char), intent(out) :: result(*)
end function

integer(C_SIZE_T) function cfs_lib_path(path) bind(C, name="lib_path")
import
character(kind=c_char), intent(out) :: path(*)
end function

logical(c_bool) function cfs_touch(path) bind(C, name="touch")
import
character(kind=c_char), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_expanduser(path, result) bind(C, name="expanduser")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: result(*)
end function

integer(C_SIZE_T) function cfs_get_homedir(path) bind(C, name="get_homedir")
import
character(kind=c_char), intent(out) :: path(*)
end function

integer(C_SIZE_T) function cfs_get_tempdir(path) bind(C, name="get_tempdir")
import
character(kind=c_char), intent(out) :: path(*)
end function

integer(C_SIZE_T) function cfs_get_cwd(path) bind(C, name="get_cwd")
import
character(kind=c_char), intent(out) :: path(*)
end function

integer(C_SIZE_T) function cfs_root(path, result) bind(C, name="root")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: result(*)
end function

integer(C_SIZE_T) function cfs_file_size(path) bind(C, name="file_size")
import
character(kind=c_char), intent(in) :: path(*)
end function

logical(c_bool) function cfs_is_exe(path) bind(C, name="is_exe")
import
character(kind=c_char), intent(in) :: path(*)
end function

logical(c_bool) function cfs_is_absolute(path) bind(C, name="is_absolute")
import
character(kind=c_char), intent(in) :: path(*)
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


module procedure filesep
character(kind=c_char) :: cbuf(2)

call cfs_filesep(cbuf)

filesep = cbuf(1)

end procedure filesep


module procedure match
match = cfs_match(trim(path) // C_NULL_CHAR, trim(pattern) // C_NULL_CHAR)
end procedure match


module procedure file_name
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_file_name(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: file_name)
file_name = cbuf(:N)

end procedure file_name


module procedure stem
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_stem(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: stem)
stem = cbuf(:N)

end procedure stem


module procedure parent
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_parent(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: parent)
parent = cbuf(:N)

end procedure parent


module procedure suffix
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_suffix(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: suffix)
suffix = cbuf(:N)

end procedure suffix


module procedure normal
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_normal(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: normal)
normal = cbuf(:N)

end procedure normal


module procedure with_suffix
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_with_suffix(trim(path) // C_NULL_CHAR, trim(new) // C_NULL_CHAR, cbuf)

allocate(character(N) :: with_suffix)
with_suffix = cbuf(:N)

end procedure with_suffix


module procedure touch
if(.not. cfs_touch(trim(path) // C_NULL_CHAR)) error stop "filesystem:touch: " // path
end procedure touch


module procedure is_absolute
!! no expanduser to be consistent with Python filesystem etc.
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


module procedure mkdir
integer :: ierr

ierr = cfs_create_directories(trim(path) // C_NULL_CHAR)
if(present(status)) then
  status = ierr
elseif (ierr /= 0) then
  error stop "ERROR:filesystem:mkdir: failed to create directory: " // path
endif
end procedure mkdir


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


module procedure root
character(kind=c_char, len=3) :: cbuf
integer(C_SIZE_T) :: N

N = cfs_root(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: root)
root = cbuf(:N)

end procedure root


module procedure exists
exists = cfs_exists(trim(path) // C_NULL_CHAR)
end procedure exists


module procedure is_file
is_file = cfs_is_file(trim(path) // C_NULL_CHAR)
end procedure is_file


module procedure is_dir
is_dir = cfs_is_dir(trim(path) // C_NULL_CHAR)
end procedure is_dir


module procedure is_exe
is_exe = cfs_is_exe(trim(path) // C_NULL_CHAR)
end procedure is_exe


module procedure same_file
same_file = cfs_equivalent(trim(path1) // C_NULL_CHAR, trim(path2) // C_NULL_CHAR)
end procedure same_file


module procedure remove
logical(c_bool) :: e

e = cfs_remove(trim(path) // C_NULL_CHAR)
if (.not. e) write(stderr, '(a)') "filesystem:unlink: " // path // " may not have been deleted."
end procedure remove


module procedure copy_file
logical(c_bool) :: ow
integer(C_INT) :: ierr

ow = .false.
if(present(overwrite)) ow = overwrite

ierr = cfs_copy_file(trim(src) // C_NULL_CHAR, trim(dest) // C_NULL_CHAR, ow)
if (present(status)) then
  status = ierr
elseif(ierr /= 0) then
  error stop "failed to copy file: " // src // " to " // dest
endif

end procedure copy_file


module procedure relative_to
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_relative_to(trim(a) // C_NULL_CHAR, trim(b) // C_NULL_CHAR, cbuf)

allocate(character(N) :: relative_to)
relative_to = cbuf(:N)

end procedure relative_to


module procedure expanduser
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_expanduser(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: expanduser)
expanduser = cbuf(:N)
end procedure expanduser


module procedure get_homedir
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_get_homedir(cbuf)

allocate(character(N) :: get_homedir)
get_homedir = cbuf(:N)

end procedure get_homedir


module procedure get_tempdir
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_get_tempdir(cbuf)

allocate(character(N) :: get_tempdir)
get_tempdir = cbuf(:N)

end procedure get_tempdir


module procedure get_cwd
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_get_cwd(cbuf)

allocate(character(N) :: get_cwd)
get_cwd = cbuf(:N)

end procedure get_cwd


module procedure lib_path
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_lib_path(cbuf)

allocate(character(N) :: lib_path)
lib_path = cbuf(:N)

end procedure lib_path


module procedure file_size
file_size = cfs_file_size(trim(path) // C_NULL_CHAR)
end procedure file_size


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


end submodule fs_cpp
