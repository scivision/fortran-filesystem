submodule (pathlib) fs_cpp

use, intrinsic :: iso_c_binding, only : c_bool, c_char, C_NULL_CHAR, C_SIZE_T

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

subroutine fs_create_directories(path) bind(C, name="create_directories")
import c_char
character(kind=c_char), intent(in) :: path(*)
end subroutine fs_create_directories

integer(C_SIZE_T) function fs_canonical(path, strict) bind(C, name="canonical")
import
character(kind=c_char), intent(inout) :: path(*)
logical(c_bool), intent(in), value :: strict
end function fs_canonical

logical(c_bool) function fs_remove(path) bind(C, name="fs_remove")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function fs_remove

logical(c_bool) function fs_exists(path) bind(C, name="exists")
import c_bool, c_char
character(kind=c_char), intent(in) :: path(*)
end function fs_exists

logical(c_bool) function fs_is_dir(path) bind(C, name="is_dir")
import
character(kind=c_char), intent(in) :: path(*)
end function fs_is_dir
logical(c_bool) function fs_equivalent(path1, path2) bind(C, name="equivalent")
import c_bool, c_char
character(kind=c_char), intent(in) :: path1(*), path2(*)
end function fs_equivalent

logical(c_bool) function fs_copy_file(source, dest, overwrite) bind(C, name="copy_file")
import
character(kind=c_char), intent(in) :: source(*), dest(*)
logical(c_bool), intent(in), value :: overwrite
end function fs_copy_file

integer(C_SIZE_T) function fs_relative_to(path, base, result) bind(C, name="relative_to")
import
character(kind=c_char), intent(in) :: path(*), base(*)
character(kind=c_char), intent(out) :: result(*)
end function fs_relative_to

logical(c_bool) function fs_touch(path) bind(C, name="touch")
import
character(kind=c_char), intent(in) :: path(*)
end function fs_touch

integer(C_SIZE_T) function fs_get_tempdir(path) bind(C, name="get_tempdir")
import
character(kind=c_char), intent(out) :: path(*)
end function fs_get_tempdir

integer(C_SIZE_T) function fs_get_cwd(path) bind(C, name="get_cwd")
import
character(kind=c_char), intent(out) :: path(*)
end function fs_get_cwd

integer(C_SIZE_T) function fs_file_size(path) bind(C, name="file_size")
import
character(kind=c_char), intent(out) :: path(*)
end function fs_file_size

end interface

contains


module procedure touch
character(kind=c_char, len=:), allocatable :: cpath

cpath = expanduser(path) // C_NULL_CHAR

if(.not. fs_touch(cpath)) error stop "pathlib:touch could not create " // path
end procedure touch


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


module procedure mkdir
character(kind=c_char, len=:), allocatable :: cpath

cpath = expanduser(path) // C_NULL_CHAR
call fs_create_directories(cpath)

end procedure mkdir


module procedure canonical
character(kind=c_char, len=2048) :: cpath
integer(C_SIZE_T) :: N, i
character(2048) :: buf
logical(c_bool) :: s

s = .false.
if(present(strict)) s = strict

cpath = expanduser(path) // C_NULL_CHAR

N = fs_canonical(cpath, s)

buf = ""
do i = 1, N
  buf(i:i) = cpath(i:i)
end do

!> C++ filesystem returns preferred separator, so make posix
canonical = as_posix(buf)

end procedure canonical


module procedure exists

character(kind=c_char, len=:), allocatable :: cpath

cpath = expanduser(path) // C_NULL_CHAR
exists = fs_exists(cpath)

end procedure exists


module procedure is_dir

character(kind=c_char, len=:), allocatable :: cpath

cpath = expanduser(path) // C_NULL_CHAR
is_dir = fs_is_dir(cpath)

end procedure is_dir

module procedure same_file
character(kind=c_char, len=:), allocatable :: c1, c2

c1 = expanduser(path1) // C_NULL_CHAR
c2 = expanduser(path2) // C_NULL_CHAR

same_file = fs_equivalent(c1, c2)

end procedure same_file


module procedure f_unlink

character(kind=c_char, len=:), allocatable :: cpath

logical(c_bool) :: e

cpath = path // C_NULL_CHAR
e = fs_remove(cpath)
if (.not. e) write(stderr, '(a)') "pathlib:unlink: " // path // " did not exist."

end procedure f_unlink


module procedure copy_file
character(kind=c_char, len=:), allocatable :: csrc, cdest

logical(c_bool) :: e, ow

ow = .false.
if(present(overwrite)) ow = overwrite

csrc = expanduser(src) // C_NULL_CHAR
cdest = expanduser(dest) // C_NULL_CHAR

e = fs_copy_file(csrc, cdest, ow)
if (.not. e) error stop "failed to copy file: " // src // " to " // dest

end procedure copy_file


module procedure relative_to

character(kind=c_char, len=:), allocatable :: s1, s2
character(:), allocatable :: a1, b1
character(kind=c_char) :: rel(2048)
integer(C_SIZE_T) :: N, i
character(2048) :: buf

a1 = expanduser(a)
b1 = expanduser(b)

!> library bug handling
if(len_trim(a1) == 0 .or. len_trim(b1) == 0) then
!! undefined case, avoid bugs with MacOS
  relative_to = ""
  return
endif

if(is_absolute(a1) .neqv. is_absolute(b1)) then
!! cannot be relative, avoid bugs with MacOS
  relative_to = ""
  return
endif

!> interface to C
s1 = a1 // C_NULL_CHAR
s2 = b1 // C_NULL_CHAR

N = fs_relative_to(s1, s2, rel)

buf = ""
do i = 1, N
  buf(i:i) = rel(i)
end do

!> C++ filesystem returns preferred separator, so make posix
relative_to = as_posix(buf)

end procedure relative_to


module procedure get_tempdir
character(kind=c_char, len=2048) :: cpath
integer(C_SIZE_T) :: N, i
character(2048) :: buf

N = fs_get_tempdir(cpath)

buf = ""
do i = 1, N
  buf(i:i) = cpath(i:i)
end do

!> C++ filesystem returns preferred separator, so make posix
get_tempdir = as_posix(buf)

end procedure get_tempdir


module procedure get_cwd
character(kind=c_char, len=2048) :: cpath
integer(C_SIZE_T) :: N, i
character(2048) :: buf

N = fs_get_cwd(cpath)

buf = ""
do i = 1, N
  buf(i:i) = cpath(i:i)
end do

!> C++ filesystem returns preferred separator, so make posix
get_cwd = as_posix(buf)

end procedure get_cwd


module procedure file_size
character(kind=c_char, len=:), allocatable :: cpath

cpath = expanduser(path) // C_NULL_CHAR

file_size = fs_file_size(cpath)
if(file_size < 0) write(stderr,*) "pathlib:file_size: " // path // " is not a file."

end procedure file_size


end submodule fs_cpp
