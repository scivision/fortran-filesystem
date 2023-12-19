module filesystem

use, intrinsic:: iso_c_binding, only: C_BOOL, C_INT, C_LONG
use, intrinsic:: iso_fortran_env, only: stderr=>error_unit, int64

implicit none
private
public :: path_t  !< base class
public :: get_homedir, canonical, resolve, get_cwd, set_cwd, make_tempdir, which !< utility procedures
public :: normal, expanduser, as_posix, as_windows, &
is_absolute, is_char_device, is_dir, is_file, is_exe, is_reserved, &
is_symlink, &
exists, &
join, &
copy_file, mkdir, &
relative_to, root, same_file, file_size, space_available, &
file_name, parent, stem, suffix, with_suffix, &
get_filename, make_absolute, &
assert_is_file, assert_is_dir, &
touch, create_symlink, &
remove, get_tempdir, &
chmod_exe, get_permissions, &
fs_cpp, fs_lang, is_admin, is_bsd, is_macos, is_windows, is_cygwin, is_wsl, is_mingw, is_linux, is_unix, &
get_max_path, exe_path, exe_dir, lib_path, lib_dir, compiler
!! functional API

!! Maximum path length is dynamically determined for this computer.
!! A fixed length eases sending data to/from C/C++.
!!
!! Physical filesystem maximum filename and path lengths are OS and config dependent.
!! Notional limits:
!! MacOS: 1024 from sys/syslimits.h PATH_MAX
!! Linux: 4096 from https://www.ibm.com/docs/en/spectrum-protect/8.1.13?topic=parameters-file-specification-syntax
!! Windows: 32767 from https://docs.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=cmd

type :: path_t

private

character(:), allocatable :: path_str

contains

procedure, public :: path=>get_path
procedure, public :: length
procedure, public :: as_posix=>f_as_posix
procedure, public :: as_windows=>f_as_windows
procedure, public :: join=>f_join
procedure, public :: relative_to=>f_relative_to
procedure, public :: normal=>f_normal
procedure, public :: exists=>f_exists
procedure, public :: is_char_device=>f_is_char_device
procedure, public :: is_file=>f_is_file
procedure, public :: is_exe=>f_is_exe
procedure, public :: is_dir=>f_is_dir
procedure, public :: is_reserved=>f_is_reserved
procedure, public :: is_absolute=>f_is_absolute
procedure, public :: is_symlink=>f_is_symlink
procedure, public :: create_symlink=>f_create_symlink
procedure, public :: copy_file=>f_copy_file
procedure, public :: mkdir=>f_mkdir
procedure, public :: touch=>f_touch
procedure, public :: parent=>f_parent
procedure, public :: file_name=>f_file_name
procedure, public :: stem=>f_stem
procedure, public :: root=>f_root
procedure, public :: suffix=>f_suffix
procedure, public :: expanduser=>f_expanduser
procedure, public :: with_suffix=>f_with_suffix
procedure, public :: canonical=>f_canonical
procedure, public :: resolve=>f_resolve
procedure, public :: same_file=>f_same_file
procedure, public :: remove=>fs_unlink
procedure, public :: file_size=>f_file_size
procedure, public :: space_available=>f_space_available
procedure, public :: chmod_exe=>f_chmod_exe
procedure, public :: get_permissions=>f_get_permissions

final :: destructor

end type path_t

interface path_t
!! constructor
  module procedure set_path
end interface


interface  !< fs_cpp.f90

integer module function get_max_path()
!! returns dynamic MAX_PATH for this computer
end function

module function as_posix(path) result(r)
!! force Posix file separator "/"
character(*), intent(in) :: path
character(:), allocatable :: r
end function

module function as_windows(path) result(r)
!! force Windows file separator "\"
character(*), intent(in) :: path
character(:), allocatable :: r
end function

module function parent(path) result(r)
!! returns parent directory of path
character(*), intent(in) :: path
character(:), allocatable :: r
end function

module function relative_to(a, b)
!! returns b relative to a
!! if b is not a subpath of a, returns "" empty string
!!
!! reference: C++ filesystem relative
!! https://en.cppreference.com/w/cpp/filesystem/relative

character(*), intent(in) :: a, b
character(:), allocatable :: relative_to
end function

module function which(name) result(r)
!! find executable in PATH
character(*), intent(in) :: name
character(:), allocatable :: r
end function

module function file_name(path)
!! returns file name without path
character(*), intent(in) :: path
character(:), allocatable :: file_name
end function

module function stem(path)
character(*), intent(in) :: path
character(:), allocatable :: stem
end function

module function suffix(path)
!! extracts path suffix, including the final "." dot
character(*), intent(in) :: path
character(:), allocatable :: suffix
end function

module function normal(path)
!! lexically normalize path
character(*), intent(in) :: path
character(:), allocatable :: normal
end function

module function with_suffix(path, new)
!! replace file suffix with new suffix
character(*), intent(in) :: path,new
character(:), allocatable :: with_suffix
end function

module logical function same_file(path1, path2)
character(*), intent(in) :: path1, path2
end function

module logical function is_file(path)
!! .true.: "path" is a file OR symlink pointing to a file
!! .false.: "path" is a directory, broken symlink, or does not exist
character(*), intent(in) :: path
end function

module function expanduser(path)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, ...
character(:), allocatable :: expanduser
character(*), intent(in) :: path
end function

module function make_absolute(path, top_path)
!! if path is absolute, return expanded path
!! if path is relative, top_path / path
!!
!! idempotent iff top_path is absolute

character(:), allocatable :: make_absolute
character(*), intent(in) :: path, top_path
end function

module function get_homedir()
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system
character(:), allocatable :: get_homedir
end function

module function canonical(path, strict)
character(:), allocatable :: canonical
character(*), intent(in) :: path
logical, intent(in), optional :: strict
end function

module function resolve(path, strict)
character(:), allocatable :: resolve
character(*), intent(in) :: path
logical, intent(in), optional :: strict
end function

module subroutine mkdir(path, status)
!! create a directory, with parents if needed
character(*), intent(in) :: path
integer, intent(out), optional :: status
end subroutine

module subroutine utime(filename)
!! like C utime(), update file modification time
character(*), intent(in) :: filename
end subroutine

module subroutine copy_file(src, dest, overwrite, status)
!! copy single file from src to dest
character(*), intent(in) :: src, dest
logical, intent(in), optional :: overwrite
integer, intent(out), optional :: status
end subroutine

module logical function is_absolute(path)
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. filesystem
character(*), intent(in) :: path
end function

module function root(path)
!! returns root of path
character(*), intent(in) :: path
character(:), allocatable :: root
end function


logical(C_BOOL) function fs_cpp() bind(C)
import C_BOOL
end function

integer(C_LONG) function fs_lang() bind(C)
import C_LONG
end function

logical(C_BOOL) function is_admin() bind(C, name="fs_is_admin")
import
end function

logical(C_BOOL) function is_bsd() bind(C, name="fs_is_bsd")
import
end function

logical(C_BOOL) function is_macos() bind(C, name="fs_is_macos")
import
end function

logical(C_BOOL) function is_windows() bind(C, name="fs_is_windows")
import
end function

logical(C_BOOL) function is_cygwin() bind(C, name="fs_is_cygwin")
import
end function

integer(C_INT) function is_wsl() bind(C, name="fs_is_wsl")
import
end function

logical(C_BOOL) function is_mingw() bind(C, name="fs_is_mingw")
import C_BOOL
end function

logical(C_BOOL) function is_linux() bind(C, name="fs_is_linux")
import C_BOOL
end function

logical(C_BOOL) function is_unix() bind(C, name="fs_is_unix")
import C_BOOL
end function

module function join(path, other)
!! returns path_t object with other appended to self using posix separator
character(:), allocatable :: join
character(*), intent(in) :: path, other
end function

module logical function is_char_device(path)
!! .true.: "path" is a character device like /dev/null
character(*), intent(in) :: path
end function

module logical function is_dir(path)
!! .true.: "path" is a directory OR symlink pointing to a directory
!! .false.: "path" is a broken symlink, does not exist, or is some other type of filesystem entity
character(*), intent(in) :: path
end function

module logical function is_reserved(path)
!! .true.: "path" is a reserved name on this platform
character(*), intent(in) :: path
end function

module integer(int64) function file_size(path)
!! size of file (bytes)
character(*), intent(in) :: path
end function

module integer(int64) function space_available(path)
!! returns space available (bytes) on filesystem containing path
character(*), intent(in) :: path
end function

module logical function is_exe(path)
!! is "path" executable?
character(*), intent(in) :: path
end function

module function get_cwd()
!! get current working directory
character(:), allocatable :: get_cwd
end function

module logical function set_cwd(path)
!! set current working directory (chdir)
character(*), intent(in) :: path
end function

module logical function is_symlink(path)
!! .true.: "path" is a symbolic link
!! .false.: "path" is not a symbolic link, or does not exist,
!!           or platform/drive not capable of symlinks
character(*), intent(in) :: path
end function

module subroutine create_symlink(tgt, link, status)
character(*), intent(in) :: tgt, link
integer, intent(out), optional :: status
end subroutine

module function compiler() result(r)
!! get compiler version from C preprocessor
character(:), allocatable :: r
end function

module function exe_path()
!! get full path of main executable
character(:), allocatable :: exe_path
end function

module function exe_dir() result(r)
!! get directory containing shared library. Empty if not shared library.
character(:), allocatable :: r
end function

module function lib_path()
!! get full path of shared library. Empty if not shared library.
character(:), allocatable :: lib_path
end function

module function lib_dir() result(r)
!! get directory containing shared library. Empty if not shared library.
character(:), allocatable :: r
end function

module logical function exists(path)
!! a file or directory exists
character(*), intent(in) :: path
end function

module subroutine remove(path)
!! delete the file, symbolic link, or empty directory
character(*), intent(in) :: path
end subroutine

module function get_tempdir()
!! get system temporary directory
character(:), allocatable :: get_tempdir
end function

module subroutine chmod_exe(path, executable, ok)
!! set owner executable bit for regular file
character(*), intent(in) :: path
logical, intent(in) :: executable
logical, intent(out), optional :: ok
end subroutine

module character(9) function get_permissions(path)
!! get permissions as POSIX string
character(*), intent(in) :: path
end function

module subroutine touch(path)
character(*), intent(in) :: path
end subroutine

module function make_tempdir() result (r)
!! make unique temporary directory
character(:), allocatable :: r
end function

end interface


interface !< find.f90
module function get_filename(path, name)
!! given a path, stem and vector of suffixes, find the full filename
!! assumes:
!! * if present, "name" is the file name we wish to find (without suffix or directories)
!! * if name not present, "path" is the directory + filename without suffix
!!
!! suffixes to check: [character(4) :: '.h5', '.nc', '.dat']
!! if file not found, empty character is returned

character(*), intent(in) :: path
character(*), intent(in), optional :: name
character(:), allocatable :: get_filename
end function
end interface


contains

subroutine destructor(self)
type(path_t), intent(inout) :: self
if(allocated(self%path_str)) deallocate(self%path_str)
end subroutine destructor

!> non-functional API

type(path_t) function set_path(path)
character(*), intent(in) :: path

allocate(character(get_max_path()) :: set_path%path_str)

set_path%path_str = path

end function set_path


pure function get_path(self, istart, iend)
character(:), allocatable :: get_path
class(path_t), intent(in) :: self
integer, intent(in), optional :: istart, iend
integer :: i1, i2

if(present(istart)) then
  i1 = istart
else
  i1 = 1
endif

if(present(iend)) then
  i2 = iend
else
  i2 = len_trim(self%path_str)
endif

get_path = self%path_str(i1:i2)

end function get_path


!> one-liner methods calling actual procedures

function f_as_posix(self) result(r)
!! force Posix "/" file separator
class(path_t), intent(in) :: self
type(path_t) :: r
r%path_str = as_posix(self%path_str)
end function

function f_as_windows(self) result(r)
!! force Windows "\" file separator
class(path_t), intent(in) :: self
type(path_t) :: r
r%path_str = as_windows(self%path_str)
end function

function f_relative_to(self, other) result(r)
!! returns other relative to self
class(path_t), intent(in) :: self
character(*), intent(in) :: other
character(:), allocatable :: r

r = relative_to(self%path_str, other)
end function


function f_stem(self) result(r)
class(path_t), intent(in) :: self
character(:), allocatable :: r
r = stem(self%path_str)
end function


function f_suffix(self) result(r)
!! extracts path suffix, including the final "." dot
class(path_t), intent(in) :: self
character(:), allocatable :: r
r = suffix(self%path_str)
end function


function f_file_name(self) result (r)
!! returns file name without path
class(path_t), intent(in) :: self
character(:), allocatable :: r
r = file_name(self%path_str)
end function


function f_parent(self) result(r)
!! returns parent directory of path
class(path_t), intent(in) :: self
character(:), allocatable :: r
r = parent(self%path_str)
end function


logical function f_is_absolute(self) result(r)
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. filesystem
class(path_t), intent(in) :: self
r = is_absolute(self%path_str)
end function


function f_with_suffix(self, new) result(r)
!! replace file suffix with new suffix
class(path_t), intent(in) :: self
type(path_t) :: r
character(*), intent(in) :: new
r%path_str = with_suffix(self%path_str, new)
end function

function f_normal(self) result(r)
!! lexically normalize path
class(path_t), intent(in) :: self
type(path_t) :: r
r%path_str = normal(self%path_str)
end function

function f_root(self) result(r)
!! returns root of path
class(path_t), intent(in) :: self
character(:), allocatable :: r
r = root(self%path_str)
end function

function f_join(self, other) result(r)
!! returns path_t object with other appended to self using posix separator
type(path_t) :: r
class(path_t), intent(in) :: self
character(*), intent(in) :: other
r%path_str = join(self%path_str, other)
end function


subroutine fs_unlink(self)
!! delete the file
class(path_t), intent(in) :: self
call remove(self%path_str)
end subroutine


logical function f_exists(self) result(r)
class(path_t), intent(in) :: self
r = exists(self%path_str)
end function


function f_resolve(self, strict) result(r)
class(path_t), intent(in) :: self
logical, intent(in), optional :: strict
type(path_t) :: r
r%path_str = resolve(self%path_str, strict)
end function

function f_canonical(self, strict) result(r)
class(path_t), intent(in) :: self
logical, intent(in), optional :: strict
type(path_t) :: r
r%path_str = canonical(self%path_str, strict)
end function


logical function f_same_file(self, other) result(r)
class(path_t), intent(in) :: self, other
r = same_file(self%path_str, other%path_str)
end function

logical function f_is_char_device(self) result(r)
class(path_t), intent(in) :: self
r = is_char_device(self%path_str)
end function

logical function f_is_dir(self) result(r)
class(path_t), intent(in) :: self
r = is_dir(self%path_str)
end function

logical function f_is_reserved(self) result(r)
class(path_t), intent(in) :: self
r = is_reserved(self%path_str)
end function

logical function f_is_symlink(self) result(r)
class(path_t), intent(in) :: self
r = is_symlink(self%path_str)
end function


subroutine f_create_symlink(self, link, status)
class(path_t), intent(in) :: self
character(*), intent(in) :: link
integer, intent(out), optional :: status
call create_symlink(self%path_str, link, status)
end subroutine


logical function f_is_file(self) result(r)
class(path_t), intent(in) :: self
r = is_file(self%path_str)
end function

integer(int64) function f_file_size(self) result(r)
class(path_t), intent(in) :: self
r = file_size(self%path_str)
end function

integer(int64) function f_space_available(self) result(r)
!! returns space available in bytes
class(path_t), intent(in) :: self
r = space_available(self%path_str)
end function


logical function f_is_exe(self) result(r)
class(path_t), intent(in) :: self
r = is_exe(self%path_str)
end function


subroutine f_mkdir(self)
!! create a directory, with parents if needed
class(path_t), intent(in) :: self
call mkdir(self%path_str)
end subroutine


subroutine f_copy_file(self, dest, overwrite)
!! copy file from source to destination
!! OVERWRITES existing destination files
class(path_t), intent(in) :: self
character(*), intent(in) :: dest
logical, intent(in), optional :: overwrite
call copy_file(self%path_str, dest, overwrite)
end subroutine


function f_expanduser(self) result(r)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, etc.
class(path_t), intent(in) :: self
type(path_t) :: r
r%path_str = expanduser(self%path_str)
end function


subroutine assert_is_file(path)
!! throw error if file does not exist
character(*), intent(in) :: path
if (is_file(path)) return
error stop 'filesystem:assert_is_file: file does not exist ' // path
end subroutine


subroutine assert_is_dir(path)
!! throw error if directory does not exist
character(*), intent(in) :: path
if (is_dir(path)) return
error stop 'filesystem:assert_is_dir: directory does not exist ' // path
end subroutine


subroutine f_touch(self)
class(path_t), intent(in) :: self
call touch(self%path_str)
end subroutine

pure integer function length(self)
!! returns string length len_trim(path)
class(path_t), intent(in) :: self
length = len_trim(self%path_str)
end function


subroutine f_chmod_exe(self, executable, ok)
class(path_t), intent(in) :: self
logical, intent(in) :: executable
logical, intent(out), optional :: ok
call chmod_exe(self%path_str, executable, ok)
end subroutine

character(9) function f_get_permissions(self)
!! get file permissions as POSIX string
class(path_t), intent(in) :: self
f_get_permissions = get_permissions(self%path_str)
end function


end module filesystem
