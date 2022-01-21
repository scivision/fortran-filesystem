submodule (filesystem) fs_cpp

use, intrinsic :: iso_c_binding, only : c_bool, c_char, c_ptr, C_NULL_CHAR, C_SIZE_T

implicit none (type, external)

interface !< fs.cpp

logical(C_BOOL) function cfs_sys_posix() bind(C, name="sys_posix")
import
end function cfs_sys_posix

integer(C_SIZE_T) function cfs_filesep(sep) bind(C, name='filesep')
import
character(kind=c_char), intent(out) :: sep(*)
end function cfs_filesep

logical(C_BOOL) function cfs_match(path, pattern) bind(C, name='match')
import
character(kind=c_char), intent(in) :: path, pattern
end function cfs_match

integer(C_SIZE_T) function cfs_as_posix(path) bind(C, name="as_posix")
import
character(kind=c_char), intent(inout) :: path(*)
end function cfs_as_posix

integer(C_SIZE_T) function cfs_file_name(path, filename) bind(C, name="file_name")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: filename(*)
end function cfs_file_name

integer(C_SIZE_T) function cfs_stem(path, fstem) bind(C, name="stem")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: fstem(*)
end function cfs_stem

integer(C_SIZE_T) function cfs_parent(path, fparent) bind(C, name="parent")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: fparent(*)
end function cfs_parent

integer(C_SIZE_T) function cfs_suffix(path, fsuffix) bind(C, name="suffix")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: fsuffix(*)
end function cfs_suffix

integer(C_SIZE_T) function cfs_with_suffix(path, new_suffix, swapped) bind(C, name="with_suffix")
import
character(kind=c_char), intent(in) :: path(*), new_suffix
character(kind=c_char), intent(out) :: swapped(*)
end function cfs_with_suffix


integer(C_SIZE_T) function cfs_normal(path, normalized) bind(C, name="normal")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: normalized(*)
end function cfs_normal

logical(c_bool) function cfs_is_symlink(path) bind(C, name="is_symlink")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function cfs_is_symlink

subroutine cfs_create_directory_symlink(target, link) bind(C, name="create_directory_symlink")
import c_char
character(kind=c_char), intent(in) :: target(*), link(*)
end subroutine cfs_create_directory_symlink

subroutine cfs_create_symlink(target, link) bind(C, name="create_symlink")
import c_char
character(kind=c_char), intent(in) :: target(*), link(*)
end subroutine cfs_create_symlink

logical(c_bool) function cfs_create_directories(path) bind(C, name="create_directories")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_create_directories

integer(C_SIZE_T) function cfs_canonical(path, strict) bind(C, name="canonical")
import
character(kind=c_char), intent(inout) :: path(*)
logical(c_bool), intent(in), value :: strict
end function cfs_canonical

logical(c_bool) function cfs_remove(path) bind(C, name="fs_remove")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function cfs_remove

logical(c_bool) function cfs_exists(path) bind(C, name="exists")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function cfs_exists

logical(c_bool) function cfs_is_file(path) bind(C, name="is_file")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_is_file

logical(c_bool) function cfs_is_dir(path) bind(C, name="is_dir")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_is_dir

logical(c_bool) function cfs_equivalent(path1, path2) bind(C, name="equivalent")
import c_bool, c_char
character(kind=c_char), intent(in) :: path1(*), path2(*)
end function cfs_equivalent

logical(c_bool) function cfs_copy_file(source, dest, overwrite) bind(C, name="copy_file")
import
character(kind=c_char), intent(in) :: source(*), dest(*)
logical(c_bool), intent(in), value :: overwrite
end function cfs_copy_file

integer(C_SIZE_T) function cfs_relative_to(path, base, result) bind(C, name="relative_to")
import
character(kind=c_char), intent(in) :: path(*), base(*)
character(kind=c_char), intent(out) :: result(*)
end function cfs_relative_to

logical(c_bool) function cfs_touch(path) bind(C, name="touch")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_touch

integer(C_SIZE_T) function cfs_expanduser(path, result) bind(C, name="expanduser")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: result(*)
end function cfs_expanduser

integer(C_SIZE_T) function cfs_get_homedir(path) bind(C, name="get_homedir")
import
character(kind=c_char), intent(out) :: path(*)
end function cfs_get_homedir

integer(C_SIZE_T) function cfs_get_tempdir(path) bind(C, name="get_tempdir")
import
character(kind=c_char), intent(out) :: path(*)
end function cfs_get_tempdir

integer(C_SIZE_T) function cfs_get_cwd(path) bind(C, name="get_cwd")
import
character(kind=c_char), intent(out) :: path(*)
end function cfs_get_cwd

integer(C_SIZE_T) function cfs_root(path, result) bind(C, name="root")
import
character(kind=c_char), intent(in) :: path(*)
character(kind=c_char), intent(out) :: result(*)
end function cfs_root

integer(C_SIZE_T) function cfs_file_size(path) bind(C, name="file_size")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_file_size

logical(c_bool) function cfs_is_exe(path) bind(C, name="is_exe")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_is_exe

logical(c_bool) function cfs_is_absolute(path) bind(C, name="is_absolute")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_is_absolute

logical(c_bool) function cfs_chmod_exe(path) bind(C, name="chmod_exe")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_chmod_exe

logical(c_bool) function cfs_chmod_no_exe(path) bind(C, name="chmod_exe")
import
character(kind=c_char), intent(in) :: path(*)
end function cfs_chmod_no_exe

end interface


contains


module procedure sys_posix
sys_posix = cfs_sys_posix()
end procedure sys_posix


module procedure filesep
character(kind=c_char) :: cbuf(3)
integer(c_size_t) :: N

N = cfs_filesep(cbuf)
if (cbuf(2) /= C_NULL_CHAR) write(stderr,'(a)') "filesystem:filesep: expected single null terminated char, got: " // cbuf(2)

filesep = cbuf(1)

end procedure filesep


module procedure match
character(kind=c_char, len=:), allocatable :: cpath, cpattern

cpath = path // C_NULL_CHAR
cpattern = pattern // C_NULL_CHAR

match = cfs_match(cpath, cpattern)
end procedure match


module procedure file_name
character(kind=c_char, len=MAXP) :: cpath, cbuf
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

cpath = path // C_NULL_CHAR

N = cfs_file_name(cpath, cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i:i)
end do

file_name = trim(buf)

end procedure file_name


module procedure stem
character(kind=c_char, len=MAXP) :: cpath, cbuf
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

cpath = path // C_NULL_CHAR

N = cfs_stem(cpath, cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i:i)
end do

stem = trim(buf)
end procedure stem


module procedure parent
character(kind=c_char, len=MAXP) :: cpath, cbuf
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

cpath = path // C_NULL_CHAR

N = cfs_parent(cpath, cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i:i)
end do

parent = trim(buf)
end procedure parent


module procedure suffix
character(kind=c_char, len=MAXP) :: cpath, cbuf
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

cpath = path // C_NULL_CHAR

N = cfs_suffix(cpath, cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i:i)
end do

suffix = trim(buf)
end procedure suffix


module procedure normal
character(kind=c_char, len=MAXP) :: cpath, cbuf
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

cpath = path // C_NULL_CHAR

N = cfs_normal(cpath, cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i:i)
end do

normal = trim(buf)
end procedure normal


module procedure as_posix
character(kind=c_char, len=MAXP) :: cbuf
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

cbuf = path // C_NULL_CHAR

N = cfs_as_posix(cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i:i)
end do

as_posix = trim(buf)
end procedure as_posix



module procedure with_suffix
character(kind=c_char, len=MAXP) :: cpath, csuff, cbuf
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

cpath = path // C_NULL_CHAR
csuff = new // C_NULL_CHAR

N = cfs_with_suffix(cpath, csuff, cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i:i)
end do

with_suffix = trim(buf)
end procedure with_suffix


module procedure touch
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR

if(.not. cfs_touch(cpath)) error stop "filesystem:touch: " // path
end procedure touch


module procedure is_absolute
!! no expanduser to be consistent with Python filesystem etc.
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR
is_absolute = cfs_is_absolute(cpath)
end procedure is_absolute


module procedure is_symlink
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR
is_symlink = cfs_is_symlink(cpath)
end procedure is_symlink


module procedure create_symlink
character(kind=c_char, len=:), allocatable :: ctgt, clink

ctgt = tgt // C_NULL_CHAR
clink = link // C_NULL_CHAR

call cfs_create_symlink(ctgt, clink)
end procedure create_symlink


module procedure mkdir
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR

if (.not. cfs_create_directories(cpath)) error stop "filesystem:mkdir: failed to create directory: " // path
end procedure mkdir


module procedure canonical
character(kind=c_char, len=MAXP) :: cpath
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf
logical(c_bool) :: s

s = .false.
if(present(strict)) s = strict

cpath = path // C_NULL_CHAR

N = cfs_canonical(cpath, s)

buf = ""
do i = 1, N
  buf(i:i) = cpath(i:i)
end do

canonical = trim(buf)

end procedure canonical


module procedure root
character(kind=c_char, len=MAXP) :: cpath, cbuf
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

cpath = path // C_NULL_CHAR

N = cfs_root(cpath, cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i:i)
end do

root = trim(buf)

end procedure root


module procedure exists
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR
exists = cfs_exists(cpath)
end procedure exists


module procedure is_file
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR
is_file = cfs_is_file(cpath)
end procedure is_file

module procedure is_dir
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR
is_dir = cfs_is_dir(cpath)
end procedure is_dir


module procedure is_exe
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR
is_exe = cfs_is_exe(cpath)
end procedure is_exe


module procedure same_file
character(kind=c_char, len=:), allocatable :: c1, c2

c1 = path1 // C_NULL_CHAR
c2 = path2 // C_NULL_CHAR

same_file = cfs_equivalent(c1, c2)
end procedure same_file


module procedure f_unlink
character(kind=c_char, len=:), allocatable :: cpath

logical(c_bool) :: e

cpath = path // C_NULL_CHAR
e = cfs_remove(cpath)
if (.not. e) write(stderr, '(a)') "filesystem:unlink: " // path // " did not exist."
end procedure f_unlink


module procedure copy_file
character(kind=c_char, len=:), allocatable :: csrc, cdest

logical(c_bool) :: e, ow

ow = .false.
if(present(overwrite)) ow = overwrite

csrc = src // C_NULL_CHAR
cdest = dest // C_NULL_CHAR

e = cfs_copy_file(csrc, cdest, ow)
if (.not. e) error stop "failed to copy file: " // src // " to " // dest
end procedure copy_file


module procedure relative_to
character(kind=c_char, len=:), allocatable :: s1, s2
character(kind=c_char) :: rel(MAXP)
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

s1 = a // C_NULL_CHAR
s2 = b // C_NULL_CHAR

N = cfs_relative_to(s1, s2, rel)

buf = ""
do i = 1, N
  buf(i:i) = rel(i)
end do

relative_to = trim(buf)
end procedure relative_to


module procedure expanduser
character(kind=c_char, len=:), allocatable :: s1
character(kind=c_char) :: cbuf(MAXP)
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

s1 = trim(path) // C_NULL_CHAR

N = cfs_expanduser(s1, cbuf)

buf = ""
do i = 1, N
  buf(i:i) = cbuf(i)
end do

expanduser = trim(buf)
end procedure expanduser


module procedure get_homedir
character(kind=c_char, len=MAXP) :: cpath
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

N = cfs_get_homedir(cpath)

buf = ""
do i = 1, N
  buf(i:i) = cpath(i:i)
end do

get_homedir = trim(buf)
end procedure get_homedir


module procedure get_tempdir
character(kind=c_char, len=MAXP) :: cpath
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

N = cfs_get_tempdir(cpath)

buf = ""
do i = 1, N
  buf(i:i) = cpath(i:i)
end do

get_tempdir = trim(buf)
end procedure get_tempdir


module procedure get_cwd
character(kind=c_char, len=MAXP) :: cpath
integer(C_SIZE_T) :: N, i
character(MAXP) :: buf

N = cfs_get_cwd(cpath)

buf = ""
do i = 1, N
  buf(i:i) = cpath(i:i)
end do

get_cwd = trim(buf)
end procedure get_cwd


module procedure file_size
character(kind=c_char, len=:), allocatable :: cpath

cpath = path // C_NULL_CHAR

file_size = cfs_file_size(cpath)
end procedure file_size


module procedure chmod_exe
character(kind=c_char, len=:), allocatable :: cpath
logical :: s

cpath = path // C_NULL_CHAR

s = cfs_chmod_exe(cpath)
if(present(ok)) ok = s
end procedure chmod_exe


module procedure chmod_no_exe
character(kind=c_char, len=:), allocatable :: cpath
logical :: s

cpath = path // C_NULL_CHAR

s = cfs_chmod_no_exe(cpath)
if(present(ok)) ok = s
end procedure chmod_no_exe


end submodule fs_cpp
