submodule (filesystem:fort2c_ifc) fs_cpp

implicit none

interface !< fs.cpp

integer(C_SIZE_T) function cfs_canonical(path, strict, canonicalized) bind(C, name="canonical")
import
character(kind=C_CHAR), intent(in) :: path(*)
logical(C_BOOL), intent(in), value :: strict
character(kind=C_CHAR), intent(out) :: canonicalized(*)
end function

integer(C_INT) function cfs_copy_file(source, dest, overwrite) bind(C, name="copy_file")
import
character(kind=c_char), intent(in) :: source(*), dest(*)
logical(c_bool), intent(in), value :: overwrite
end function

integer(C_INT) function cfs_create_directories(path) bind(C, name="create_directories")
import
character(kind=C_CHAR), intent(in) :: path(*)
end function

logical(C_BOOL) function cfs_equivalent(path1, path2) bind(C, name="equivalent")
import C_BOOL, C_CHAR
character(kind=C_CHAR), intent(in) :: path1(*), path2(*)
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

integer(C_SIZE_T) function cfs_get_homedir(path) bind(C, name="get_homedir")
import
character(kind=c_char), intent(out) :: path(*)
end function

integer(C_SIZE_T) function cfs_get_tempdir(path) bind(C, name="get_tempdir")
import
character(kind=c_char), intent(out) :: path(*)
end function

logical(C_BOOL) function cfs_match(path, pattern) bind(C, name='match')
import
character(kind=c_char), intent(in) :: path, pattern
end function

integer(C_SIZE_T) function cfs_relative_to(path, base, result) bind(C, name="relative_to")
import
character(kind=c_char), intent(in) :: path(*), base(*)
character(kind=c_char), intent(out) :: result(*)
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

logical(c_bool) function cfs_touch(path) bind(C, name="touch")
import
character(kind=c_char), intent(in) :: path(*)
end function

integer(C_SIZE_T) function cfs_with_suffix(path, new_suffix, swapped) bind(C, name="with_suffix")
import
character(kind=C_CHAR), intent(in) :: path(*), new_suffix
character(kind=C_CHAR), intent(out) :: swapped(*)
end function

end interface

contains

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

module procedure expanduser
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_expanduser(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: expanduser)
expanduser = cbuf(:N)
end procedure expanduser


module procedure file_name
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_file_name(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: file_name)
file_name = cbuf(:N)

end procedure file_name

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


module procedure match
match = cfs_match(trim(path) // C_NULL_CHAR, trim(pattern) // C_NULL_CHAR)
end procedure

module procedure mkdir
integer :: ierr

ierr = cfs_create_directories(trim(path) // C_NULL_CHAR)
if(present(status)) then
  status = ierr
elseif (ierr /= 0) then
  error stop "ERROR:filesystem:mkdir: failed to create directory: " // path
endif
end procedure mkdir

module procedure relative_to
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_relative_to(trim(a) // C_NULL_CHAR, trim(b) // C_NULL_CHAR, cbuf)

allocate(character(N) :: relative_to)
relative_to = cbuf(:N)

end procedure relative_to

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

end procedure stem

module procedure suffix
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N

allocate(character(max_path()) :: cbuf)

N = cfs_suffix(trim(path) // C_NULL_CHAR, cbuf)

allocate(character(N) :: suffix)
suffix = cbuf(:N)

end procedure suffix

module procedure touch
if(.not. cfs_touch(trim(path) // C_NULL_CHAR)) error stop "filesystem:touch: " // path
end procedure

module procedure with_suffix
character(kind=c_char, len=:), allocatable :: cbuf
integer(C_SIZE_T) :: N
allocate(character(max_path()) :: cbuf)
N = cfs_with_suffix(trim(path) // C_NULL_CHAR, trim(new) // C_NULL_CHAR, cbuf)
allocate(character(N) :: with_suffix)
with_suffix = cbuf(:N)
end procedure with_suffix

end submodule fs_cpp
