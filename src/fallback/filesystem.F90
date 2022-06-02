module filesystem

use, intrinsic:: iso_c_binding, only: C_BOOL, C_CHAR, C_NULL_CHAR, C_SIZE_T
use, intrinsic:: iso_fortran_env, only: stderr=>error_unit, int64

implicit none (type, external)
private
public :: path_t  !< base class
public :: get_cwd !< utility procedures
public :: as_posix, expanduser, &
is_absolute, is_dir, is_file, exists, get_homedir, get_tempdir, &
join, filesep, &
copy_file, mkdir, &
file_parts, relative_to, resolve, root, same_file, file_size, &
file_name, parent, stem, suffix, with_suffix, &
assert_is_file, assert_is_dir, &
sys_posix, touch, &
remove, get_filename, make_absolute, &
is_macos, is_linux, is_unix, is_windows, &
is_symlink, create_symlink, is_exe, normal
!! functional API

integer, public, protected :: MAXP = 4096
!! arbitrary maximum path length.
!! We use a fixed length to ease sending data to/from C/C++.
!! We could make this dynamic if this fixed length becomes an issue.
!!
!! Physical filesystem maximum filename and path lengths are OS and config dependent.
!! Notional limits:
!! MacOS: 1024
!! Linux: 4096
!! https://www.ibm.com/docs/en/spectrum-protect/8.1.13?topic=parameters-file-specification-syntax
!! Windows: 32767
!! https://docs.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=cmd

interface remove
  module procedure f_unlink
end interface remove

interface resolve
  module procedure canonical
end interface resolve


type :: path_t

private

character(:), allocatable :: path_str

contains

procedure, public :: path=>get_path
procedure, public :: length
procedure, public :: join=>fs_join
procedure, public :: parts=>fs_parts
procedure, public :: relative_to=>fs_relative_to
procedure, public :: exists=>fs_exists
procedure, public :: is_file=>fs_is_file
procedure, public :: is_dir=>fs_is_dir
procedure, public :: is_absolute=>fs_is_absolute
procedure, public :: copy_file=>fs_copy_file
procedure, public :: mkdir=>fs_mkdir
procedure, public :: touch=>fs_touch
procedure, public :: parent=>fs_parent
procedure, public :: file_name=>fs_file_name
procedure, public :: stem=>fs_stem
procedure, public :: root=>fs_root
procedure, public :: suffix=>fs_suffix
procedure, public :: as_posix=>fs_as_posix
procedure, public :: expanduser=>fs_expanduser
procedure, public :: with_suffix=>fs_with_suffix
procedure, public :: resolve=>fs_resolve
procedure, public :: same_file=>fs_same_file
procedure, public :: remove=>fs_unlink
procedure, public :: file_size=>fs_file_size

procedure, public :: is_symlink=>fs_is_symlink
procedure, public :: create_symlink=>fs_create_symlink
procedure, public :: normal=>fs_normal

end type path_t


interface path_t
  module procedure set_path
end interface

interface !< find.f90

module function get_filename(path, name, suffixes)
!! given a path, stem and vector of suffixes, find the full filename
!! assumes:
!! * if present, "name" is the file name we wish to find (without suffix or directories)
!! * if name not present, "path" is the directory + filename without suffix
!!
!! suffixes is a vector of suffixes to check. Default is [character(4) :: '.h5', '.nc', '.dat']
!! if file not found, empty character is returned

character(*), intent(in) :: path
character(*), intent(in), optional :: name, suffixes(:)
character(:), allocatable :: get_filename
end function

module function make_absolute(path, top_path)
!! if path is absolute, return expanded path
!! if path is relative, top_path / path
!!
!! idempotent iff top_path is absolute

character(:), allocatable :: make_absolute
character(*), intent(in) :: path, top_path
end function

end interface

interface !< general.f90

module function get_homedir()
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system
character(:), allocatable :: get_homedir
end function

module function get_tempdir()
!! returns temp directory, or empty string if not found
!!
character(:), allocatable :: get_tempdir
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

module subroutine touch(path)
character(*), intent(in) :: path
end subroutine

module logical function same_file(path1, path2)
character(*), intent(in) :: path1, path2
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

module function parent(path)
!! returns parent directory of path
character(*), intent(in) :: path
character(:), allocatable :: parent
end function

module function suffix(path)
!! extracts path suffix, including the final "." dot
character(*), intent(in) :: path
character(:), allocatable :: suffix
end function

module function with_suffix(path, new)
!! replace file suffix with new suffix
character(*), intent(in) :: path,new
character(:), allocatable :: with_suffix
end function

end interface


interface !< filesystem.c

logical(C_BOOL) function is_macos() bind(C)
import C_BOOL
end function

logical(C_BOOL) function is_windows() bind(C)
import C_BOOL
end function

logical(C_BOOL) function is_linux() bind(C)
import C_BOOL
end function

logical(C_BOOL) function is_unix() bind(C)
import C_BOOL
end function

logical(C_BOOL) function sys_posix() bind(C)
import C_BOOL
end function

end interface


interface !< compiler/{intel,gcc}

module function as_posix(path)
!! '\' => '/', dropping redundant separators

character(:), allocatable :: as_posix
character(*), intent(in) :: path
end function

module logical function is_dir(path)
!! .true.: "path" is a directory OR symlink pointing to a directory
!! .false.: "path" is a broken symlink, does not exist, or is some other type of filesystem entity
character(*), intent(in) :: path
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

module function get_cwd()
character(:), allocatable :: get_cwd
end function

module subroutine f_unlink(path)
!! delete the file, symbolic link, or empty directory
character(*), intent(in) :: path
end subroutine

module integer(int64) function file_size(path)
character(*), intent(in) :: path
end function

module function root(path)
!! returns root of path
character(*), intent(in) :: path
character(:), allocatable :: root
end function

module character function filesep()
!! get system file separator
end function

end interface


interface  ! {posix,windows}/crt.f90
!! C Runtime Library procedures

module function canonical(path, strict)
character(:), allocatable :: canonical
character(*), intent(in) :: path
logical, intent(in), optional :: strict
end function

module subroutine mkdir(path)
!! create a directory, with parents if needed
character(*), intent(in) :: path
end subroutine

module subroutine file_parts(path, fparts)
!! split path into up to 1000 parts (arbitrary limit)
!! all path separators are discarded, except the leftmost if present
character(*), intent(in) :: path
character(:), allocatable, intent(out) :: fparts(:)
!! allocatable, intent(out) because we do want to implicitly deallocate first
end subroutine


end interface


interface !< {posix,windows}/sys.f90
module subroutine copy_file(src, dest, overwrite)
!! copy single file from src to dest
!! OVERWRITES existing destination file
character(*), intent(in) :: src, dest
logical, intent(in), optional :: overwrite
end subroutine
end interface


interface !< {posix,windows}/path.f90

module logical function is_absolute(path)
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. filesystem
character(*), intent(in) :: path
end function

end interface


contains

!> non-existent

logical function fs_is_symlink(self)
class(path_t), intent(in) :: self
fs_is_symlink = is_symlink(self%path_str)
end function

logical function is_symlink(path)
character(*), intent(in) :: path
write(stderr,'(a)') "WARNING:filesystem:fallback doesn't have is_symlink"
is_symlink = .false.
end function


subroutine fs_create_symlink(self, target, status)
class(path_t), intent(in) :: self
character(*), intent(in) :: target
integer, intent(out), optional :: status
call create_symlink(self%path_str, target, status)
end subroutine fs_create_symlink

subroutine create_symlink(tgt, link, status)
character(*), intent(in) :: tgt, link
integer, intent(out), optional :: status
if (present(status)) then
  status = -1
  return
endif
error stop "filesystem:fallback doesn't have create_symlink"
end subroutine


logical function is_exe(path)
character(*), intent(in) :: path
error stop "filesystem:fallback doesn't have is_exe"
end function

function fs_normal(self)
!! lexically normalize path
class(path_t), intent(in) :: self
type(path_t) :: fs_normal

fs_normal%path_str = normal(self%path_str)
end function

function normal(path)
character(*), intent(in) :: path
character(:), allocatable :: normal
error stop "filesystem:fallback doesn't have normal"
end function

!> one-liner methods calling actual procedures

function fs_relative_to(self, other)
!! returns other relative to self
class(path_t), intent(in) :: self
character(*), intent(in) :: other
character(:), allocatable :: fs_relative_to

fs_relative_to = relative_to(self%path_str, other)
end function fs_relative_to


function fs_stem(self)
class(path_t), intent(in) :: self
character(:), allocatable :: fs_stem

fs_stem = stem(self%path_str)
end function fs_stem


function fs_suffix(self)
!! extracts path suffix, including the final "." dot
class(path_t), intent(in) :: self
character(:), allocatable :: fs_suffix

fs_suffix = suffix(self%path_str)
end function fs_suffix


function fs_file_name(self)
!! returns file name without path
class(path_t), intent(in) :: self
character(:), allocatable :: fs_file_name

fs_file_name = file_name(self%path_str)
end function fs_file_name


function fs_parent(self)
!! returns parent directory of path
class(path_t), intent(in) :: self
character(:), allocatable :: fs_parent

fs_parent = parent(self%path_str)
end function fs_parent


logical function fs_is_absolute(self)
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. filesystem
class(path_t), intent(in) :: self

fs_is_absolute = is_absolute(self%path_str)
end function fs_is_absolute


function fs_with_suffix(self, new)
!! replace file suffix with new suffix
class(path_t), intent(in) :: self
type(path_t) :: fs_with_suffix
character(*), intent(in) :: new

fs_with_suffix%path_str = with_suffix(self%path_str, new)
end function fs_with_suffix


function fs_root(self)
!! returns root of path
class(path_t), intent(in) :: self
character(:), allocatable :: fs_root

fs_root = root(self%path_str)
end function fs_root


function fs_as_posix(self)
!! '\' => '/', dropping redundant separators
class(path_t), intent(in) :: self
type(path_t) :: fs_as_posix

fs_as_posix%path_str = as_posix(self%path_str)
end function fs_as_posix


function fs_join(self, other)
!! returns path_t object with other appended to self using posix separator
type(path_t) :: fs_join
class(path_t), intent(in) :: self
character(*), intent(in) :: other

fs_join%path_str = join(self%path_str, other)
end function fs_join


subroutine fs_unlink(self)
!! delete the file
class(path_t), intent(in) :: self

call f_unlink(self%path_str)
end subroutine


logical function fs_exists(self)
class(path_t), intent(in) :: self

fs_exists = exists(self%path_str)
end function fs_exists


function fs_resolve(self)
class(path_t), intent(in) :: self
type(path_t) :: fs_resolve

fs_resolve%path_str = resolve(self%path_str)
end function fs_resolve


logical function fs_same_file(self, other)
class(path_t), intent(in) :: self, other

fs_same_file = same_file(self%path_str, other%path_str)
end function fs_same_file


logical function fs_is_dir(self)
class(path_t), intent(in) :: self

fs_is_dir = is_dir(self%path_str)
end function fs_is_dir


logical function fs_is_file(self)
class(path_t), intent(in) :: self

fs_is_file = is_file(self%path_str)
end function fs_is_file

integer(int64) function fs_file_size(self)
class(path_t), intent(in) :: self

fs_file_size = file_size(self%path_str)
end function fs_file_size


subroutine fs_mkdir(self)
!! create a directory, with parents if needed
class(path_t), intent(in) :: self

call mkdir(self%path_str)
end subroutine


subroutine fs_copy_file(self, dest, overwrite)
!! copy file from source to destination
!! OVERWRITES existing destination files
class(path_t), intent(in) :: self
character(*), intent(in) :: dest
logical, intent(in), optional :: overwrite

call copy_file(self%path_str, dest, overwrite)
end subroutine


function fs_expanduser(self)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, etc.
class(path_t), intent(in) :: self
type(path_t) :: fs_expanduser

fs_expanduser%path_str = expanduser(self%path_str)
end function fs_expanduser

function fs_parts(self)
!! split path into up to 1000 parts (arbitrary limit)
!! all path separators are discarded, except the leftmost if present
class(path_t), intent(in) :: self
character(:), allocatable :: fs_parts(:)

call file_parts(self%path_str, fparts=fs_parts)
end function fs_parts


subroutine fs_touch(self)
class(path_t), intent(in) :: self

call touch(self%path_str)
end subroutine

!! non-functional API

pure function set_path(path)
type(path_t) :: set_path
character(*), intent(in) :: path
set_path%path_str = trim(path)
end function set_path


pure function get_path(self, istart, iend)
character(:), allocatable :: get_path
class(path_t), intent(in) :: self
integer, intent(in), optional :: istart, iend
integer :: i1, i2

i1 = 1
i2 = len_trim(self%path_str)

if(present(istart)) i1 = istart
if(present(iend)) i2 = iend

get_path = self%path_str(i1:i2)

end function get_path


pure integer function length(self)
!! returns string length len_trim(path)
class(path_t), intent(in) :: self

length = len_trim(self%path_str)
end function length


function join(path, other)
!! returns path_t object with other appended to self using posix separator
character(:), allocatable :: join
character(*), intent(in) :: path, other

join = as_posix(path // "/" // other)
end function join


subroutine assert_is_file(path)
!! throw error if file does not exist
character(*), intent(in) :: path

if (is_file(path)) return

error stop 'filesystem:assert_is_file: file does not exist ' // path
end subroutine assert_is_file


subroutine assert_is_dir(path)
!! throw error if directory does not exist
character(*), intent(in) :: path

if (is_dir(path)) return

error stop 'filesystem:assert_is_dir: directory does not exist ' // path
end subroutine assert_is_dir


logical function exists(path)
character(*), intent(in) :: path
exists = (is_dir(path) .or. is_file(path))
end function exists

end module filesystem


!> switchyard for OS-specific procedures
#ifdef _WIN32
include "windows/sys.inc"
#else
include "posix/sys.inc"
#endif

!> switchyard for compiler-specific procedures
#ifdef __GFORTRAN__
include "compiler/gcc.inc"
#elif defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)
include "compiler/intel.inc"
#endif

!> procedures from main filesystem
include "../iter.f90"
include "../find.f90"
