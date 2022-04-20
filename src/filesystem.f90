module filesystem

use, intrinsic:: iso_fortran_env, only: stderr=>error_unit, int64

implicit none (type, external)
private
public :: path_t  !< base class
public :: get_homedir, canonical, get_cwd !< utility procedures
public :: as_posix, normal, expanduser, &
is_absolute, is_dir, is_file, is_exe, &
filesystem_has_symlink, filesystem_has_normalize, filesystem_has_relative_to, filesystem_has_weakly_canonical, &
is_symlink, &
exists, match, &
join, &
copy_file, mkdir, &
file_parts, relative_to, resolve, root, same_file, file_size, &
file_name, parent, stem, suffix, with_suffix, &
read_text, write_text, &
get_filename, make_absolute, &
assert_is_file, assert_is_dir, &
sys_posix, touch, create_symlink, &
remove, get_tempdir, filesep, &
chmod_exe, chmod_no_exe, &
is_macos, is_windows, is_linux, is_unix
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

procedure, public :: path=>get_path, &
length, join=>fs_join, parts=>fs_parts, relative_to=>fs_relative_to, &
normal=>fs_normal, &
exists=>fs_exists, match=>fs_match, &
is_file=>fs_is_file, is_dir=>fs_is_dir, is_absolute=>fs_is_absolute, &
is_symlink=>fs_is_symlink, create_symlink=>fs_create_symlink, &
copy_file=>fs_copy_file, mkdir=>fs_mkdir, &
touch=>fs_touch, &
parent=>fs_parent, file_name=>fs_file_name, stem=>fs_stem, &
root=>fs_root, suffix=>fs_suffix, &
as_posix=>fs_as_posix, expanduser=>fs_expanduser, &
with_suffix=>fs_with_suffix, &
resolve=>fs_resolve, same_file=>fs_same_file, is_exe=>fs_is_exe, &
remove=>fs_unlink, file_size=>fs_file_size, &
read_text=>fs_read_text, write_text=>fs_write_text, &
chmod_exe=>fs_chmod_exe, chmod_no_exe=>fs_chmod_no_exe

end type path_t

interface path_t
  module procedure set_path
end interface


interface  !< pure.f90
module function parent(path)
!! returns parent directory of path
character(*), intent(in) :: path
character(:), allocatable :: parent
end function parent

module function relative_to(a, b)
!! returns b relative to a
!! if b is not a subpath of a, returns "" empty string
!!
!! reference: C++ filesystem relative
!! https://en.cppreference.com/w/cpp/filesystem/relative

character(*), intent(in) :: a, b
character(:), allocatable :: relative_to
end function relative_to

module function file_name(path)
!! returns file name without path
character(*), intent(in) :: path
character(:), allocatable :: file_name
end function file_name

module function stem(path)
character(*), intent(in) :: path
character(:), allocatable :: stem
end function stem

module function suffix(path)
!! extracts path suffix, including the final "." dot
character(*), intent(in) :: path
character(:), allocatable :: suffix
end function suffix

module function as_posix(path)
!! '\' => '/', dropping redundant separators

character(:), allocatable :: as_posix
character(*), intent(in) :: path
end function as_posix

module function normal(path)
!! lexically normalize path
character(*), intent(in) :: path
character(:), allocatable :: normal
end function normal

module function with_suffix(path, new)
!! replace file suffix with new suffix
character(*), intent(in) :: path,new
character(:), allocatable :: with_suffix
end function with_suffix

end interface !< pure.f90


interface  !< pure_iter.f90

module subroutine file_parts(path, fparts)
!! split path into up to 1000 parts (arbitrary limit)
!! all path separators are discarded, except the leftmost if present
character(*), intent(in) :: path
character(:), allocatable, intent(out) :: fparts(:)
!! allocatable, intent(out) because we do want to implicitly deallocate first
end subroutine file_parts

end interface


interface !< impure.f90
module logical function same_file(path1, path2)
character(*), intent(in) :: path1, path2
end function same_file

module logical function is_file(path)
!! .true.: "path" is a file OR symlink pointing to a file
!! .false.: "path" is a directory, broken symlink, or does not exist
character(*), intent(in) :: path
end function is_file

module function expanduser(path)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, ...
character(:), allocatable :: expanduser
character(*), intent(in) :: path
end function expanduser

end interface  !< impure.f90


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
end function get_filename

module function make_absolute(path, top_path)
!! if path is absolute, return expanded path
!! if path is relative, top_path / path
!!
!! idempotent iff top_path is absolute

character(:), allocatable :: make_absolute
character(*), intent(in) :: path, top_path
end function make_absolute

end interface


interface !< io.f90

module subroutine touch(path)
character(*), intent(in) :: path
end subroutine touch

module function read_text(filename, max_length)
!! read text file
character(*), intent(in) :: filename
character(:), allocatable :: read_text
integer, optional :: max_length
end function read_text

module subroutine write_text(filename, text)
!! create or overwrite file with text
character(*), intent(in) :: filename, text
end subroutine write_text

end interface


interface !< envvar.f90
module function get_homedir()
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system
character(:), allocatable :: get_homedir
end function get_homedir


end interface


interface

module function canonical(path, strict)
character(:), allocatable :: canonical
character(*), intent(in) :: path
logical, intent(in), optional :: strict
end function canonical

module subroutine mkdir(path)
!! create a directory, with parents if needed
character(*), intent(in) :: path
end subroutine mkdir

module subroutine utime(filename)
!! like C utime(), update file modification time
character(*), intent(in) :: filename
end subroutine utime

end interface


interface
module subroutine copy_file(src, dest, overwrite)
!! copy single file from src to dest
!! OVERWRITES existing destination file
character(*), intent(in) :: src, dest
logical, intent(in), optional :: overwrite
end subroutine copy_file
end interface


interface
module logical function is_absolute(path)
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. filesystem
character(*), intent(in) :: path
end function is_absolute

module function root(path)
!! returns root of path
character(*), intent(in) :: path
character(:), allocatable :: root
end function root

module logical function sys_posix()
end function sys_posix

end interface


interface

module logical function is_dir(path)
!! .true.: "path" is a directory OR symlink pointing to a directory
!! .false.: "path" is a broken symlink, does not exist, or is some other type of filesystem entity
character(*), intent(in) :: path
end function is_dir

module integer(int64) function file_size(path)
character(*), intent(in) :: path
end function file_size

module logical function is_exe(path)
character(*), intent(in) :: path
end function is_exe

module function get_cwd()
character(:), allocatable :: get_cwd
end function get_cwd

end interface


interface !< fs_cpp.f90

module logical function is_macos()
end function is_macos

module logical function is_windows()
end function is_windows

module logical function is_linux()
end function is_linux

module logical function is_unix()
end function is_unix

module logical function filesystem_has_symlink()
end function filesystem_has_symlink

module logical function filesystem_has_weakly_canonical()
end function filesystem_has_weakly_canonical

module logical function filesystem_has_normalize()
end function filesystem_has_normalize

module logical function filesystem_has_relative_to()
end function filesystem_has_relative_to

module logical function is_symlink(path)
!! .true.: "path" is a symbolic link
!! .false.: "path" is not a symbolic link, or does not exist
character(*), intent(in) :: path
end function is_symlink

module subroutine create_symlink(tgt, link)
character(*), intent(in) :: tgt, link
end subroutine create_symlink

module logical function exists(path)
!! a file or directory exists
character(*), intent(in) :: path
end function exists


module logical function match(path, pattern)
!! does any substring of path match the pattern
!! pattern uses C++ regex_search() syntax
character(*), intent(in) :: path, pattern
end function match


module subroutine f_unlink(path)
!! delete the file, symbolic link, or empty directory
character(*), intent(in) :: path
end subroutine f_unlink

module function get_tempdir()
!! get system temporary directory
character(:), allocatable :: get_tempdir
end function get_tempdir

module character function filesep()
!! get system file separator
end function filesep

module subroutine chmod_exe(path, ok)
!! set owner executable bit for regular file
character(*), intent(in) :: path
logical, intent(out), optional :: ok
end subroutine chmod_exe

module subroutine chmod_no_exe(path, ok)
!! set owner non-executable bit for regular file
character(*), intent(in) :: path
logical, intent(out), optional :: ok
end subroutine chmod_no_exe


end interface


contains

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

function fs_normal(self)
!! lexically normalize path
class(path_t), intent(in) :: self
type(path_t) :: fs_normal

fs_normal%path_str = normal(self%path_str)
end function fs_normal

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


logical function fs_match(self, pattern)
!! does any substring of path match the pattern
!! pattern uses C++ regex_search() syntax
class(path_t), intent(in) :: self
character(*), intent(in) :: pattern

fs_match = match(self%path_str, pattern)
end function fs_match


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
end subroutine fs_unlink


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


logical function fs_is_symlink(self)
class(path_t), intent(in) :: self

fs_is_symlink = is_symlink(self%path_str)
end function fs_is_symlink


subroutine fs_create_symlink(self, link)
class(path_t), intent(in) :: self
character(*), intent(in) :: link

call create_symlink(self%path_str, link)
end subroutine fs_create_symlink


logical function fs_is_file(self)
class(path_t), intent(in) :: self

fs_is_file = is_file(self%path_str)
end function fs_is_file

integer(int64) function fs_file_size(self)
class(path_t), intent(in) :: self

fs_file_size = file_size(self%path_str)
end function fs_file_size


logical function fs_is_exe(self)
class(path_t), intent(in) :: self

fs_is_exe = is_exe(self%path_str)
end function fs_is_exe


subroutine fs_mkdir(self)
!! create a directory, with parents if needed
class(path_t), intent(in) :: self

call mkdir(self%path_str)
end subroutine fs_mkdir


subroutine fs_copy_file(self, dest, overwrite)
!! copy file from source to destination
!! OVERWRITES existing destination files
class(path_t), intent(in) :: self
character(*), intent(in) :: dest
logical, intent(in), optional :: overwrite

call copy_file(self%path_str, dest, overwrite)
end subroutine fs_copy_file


function fs_expanduser(self)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, etc.
class(path_t), intent(in) :: self
type(path_t) :: fs_expanduser

fs_expanduser%path_str = expanduser(self%path_str)
end function fs_expanduser


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
end subroutine fs_touch


function fs_read_text(self, max_length)
!! read text file
class(path_t), intent(in) :: self
character(:), allocatable :: fs_read_text
integer, optional :: max_length

fs_read_text = read_text(self%path_str, max_length)
end function fs_read_text


subroutine fs_write_text(self, text)
!! create or overwrite file with text
class(path_t), intent(in) :: self
character(*), intent(in) :: text

call write_text(self%path_str, text)
end subroutine fs_write_text


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


subroutine fs_chmod_exe(self)
class(path_t), intent(in) :: self

call chmod_exe(self%path_str)
end subroutine fs_chmod_exe


subroutine fs_chmod_no_exe(self)
class(path_t), intent(in) :: self

call chmod_no_exe(self%path_str)
end subroutine fs_chmod_no_exe


end module filesystem
