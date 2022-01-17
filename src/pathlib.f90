module pathlib

use, intrinsic:: iso_fortran_env, only: stderr=>error_unit, int64

implicit none (type, external)
private
public :: path_t  !< base class
public :: home, canonical, get_cwd !< utility procedures
public :: as_posix, as_windows, drop_sep, expanduser, &
is_absolute, is_dir, is_file, is_exe, is_symlink, exists, &
join, &
copy_file, mkdir, &
file_parts, relative_to, resolve, root, same_file, file_size, &
file_name, parent, stem, suffix, with_suffix, &
read_text, write_text, &
get_filename, make_absolute, &
assert_is_file, assert_is_dir, &
sys_posix, touch, create_symlink, &
remove, get_tempdir
!! functional API

interface remove
  module procedure f_unlink
end interface remove

type :: path_t

private

character(:), allocatable :: path_str

contains

procedure, public :: path=>get_path, &
length, join=>pathlib_join, parts=>pathlib_parts, relative_to=>pathlib_relative_to, &
drop_sep=>pathlib_drop_sep, &
exists=>pathlib_exists, &
is_file=>pathlib_is_file, is_dir=>pathlib_is_dir, is_absolute=>pathlib_is_absolute, &
is_symlink=>pathlib_is_symlink, create_symlink=>pathlib_create_symlink, &
copy_file=>pathlib_copy_file, mkdir=>pathlib_mkdir, &
touch=>pathlib_touch, &
parent=>pathlib_parent, file_name=>pathlib_file_name, stem=>pathlib_stem, root=>pathlib_root, suffix=>pathlib_suffix, &
as_windows=>pathlib_as_windows, as_posix=>pathlib_as_posix, expanduser=>pathlib_expanduser, &
with_suffix=>pathlib_with_suffix, &
resolve=>pathlib_resolve, same_file=>pathlib_same_file, is_exe=>pathlib_is_exe, &
remove=>pathlib_unlink, file_size=>pathlib_file_size, &
read_text=>pathlib_read_text, write_text=>pathlib_write_text

end type path_t

interface path_t
  module procedure set_path
end interface


interface  !< pure.f90
module pure integer function length(self)
!! returns string length len_trim(path)
class(path_t), intent(in) :: self
end function length

module pure function pathlib_join(self, other)
!! returns path_t object with other appended to self using posix separator
type(path_t) :: pathlib_join
class(path_t), intent(in) :: self
character(*), intent(in) :: other
end function pathlib_join

module pure function join(path, other)
!! returns path_t object with other appended to self using posix separator
character(:), allocatable :: join
character(*), intent(in) :: path, other
end function join

module pure function pathlib_as_windows(self)
!! '/' => '\' for Windows systems

class(path_t), intent(in) :: self
type(path_t) :: pathlib_as_windows
end function pathlib_as_windows

module pure function as_windows(path)
!! '/' => '\' for Windows systems
character(*), intent(in) :: path
character(:), allocatable :: as_windows
end function as_windows

module pure function pathlib_parent(self)
!! returns parent directory of path
class(path_t), intent(in) :: self
character(:), allocatable :: pathlib_parent
end function pathlib_parent

module pure function parent(path)
!! returns parent directory of path
character(*), intent(in) :: path
character(:), allocatable :: parent
end function parent


module impure function pathlib_relative_to(self, other)
!! returns other relative to self
class(path_t), intent(in) :: self
character(*), intent(in) :: other
character(:), allocatable :: pathlib_relative_to
end function pathlib_relative_to

module impure function relative_to(a, b)
!! returns b relative to a
!! if b is not a subpath of a, returns "" empty string
!!
!! reference: C++17 filesystem relative
!! https://en.cppreference.com/w/cpp/filesystem/relative

character(*), intent(in) :: a, b
character(:), allocatable :: relative_to
end function relative_to


module pure function pathlib_file_name(self)
!! returns file name without path
class(path_t), intent(in) :: self
character(:), allocatable :: pathlib_file_name
end function pathlib_file_name
module pure function file_name(path)
!! returns file name without path
character(*), intent(in) :: path
character(:), allocatable :: file_name
end function file_name


module pure function pathlib_stem(self)
class(path_t), intent(in) :: self

character(:), allocatable :: pathlib_stem
end function pathlib_stem
module pure function stem(path)
character(*), intent(in) :: path
character(:), allocatable :: stem
end function stem

module pure function pathlib_suffix(self)
!! extracts path suffix, including the final "." dot
class(path_t), intent(in) :: self
character(:), allocatable :: pathlib_suffix
end function pathlib_suffix

module pure function suffix(path)
!! extracts path suffix, including the final "." dot
character(*), intent(in) :: path
character(:), allocatable :: suffix
end function suffix
module pure function pathlib_as_posix(self)
!! '\' => '/', dropping redundant separators

class(path_t), intent(in) :: self
type(path_t) :: pathlib_as_posix
end function pathlib_as_posix

module pure function as_posix(path)
!! '\' => '/', dropping redundant separators

character(:), allocatable :: as_posix
character(*), intent(in) :: path
end function as_posix

module pure function pathlib_drop_sep(self)
!! drop redundant "/" file separators
class(path_t), intent(in) :: self
type(path_t) :: pathlib_drop_sep
end function pathlib_drop_sep

module pure function drop_sep(path)
!! drop redundant "/" file separators
character(*), intent(in) :: path
character(:), allocatable :: drop_sep
end function drop_sep

module pure function pathlib_with_suffix(self, new)
!! replace file suffix with new suffix
class(path_t), intent(in) :: self
type(path_t) :: pathlib_with_suffix
character(*), intent(in) :: new
end function pathlib_with_suffix

module pure function with_suffix(path, new)
!! replace file suffix with new suffix
character(*), intent(in) :: path,new
character(:), allocatable :: with_suffix
end function with_suffix

module pure logical function pathlib_is_absolute(self)
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib
class(path_t), intent(in) :: self
end function pathlib_is_absolute

module pure function pathlib_root(self)
!! returns root of path
class(path_t), intent(in) :: self
character(:), allocatable :: pathlib_root
end function pathlib_root

end interface !< pure.f90


interface  !< pure_iter.f90

module pure function pathlib_parts(self)
!! split path into up to 1000 parts (arbitrary limit)
!! all path separators are discarded, except the leftmost if present
class(path_t), intent(in) :: self
character(:), allocatable :: pathlib_parts(:)
end function pathlib_parts

module pure subroutine file_parts(path, fparts)
!! split path into up to 1000 parts (arbitrary limit)
!! all path separators are discarded, except the leftmost if present
character(*), intent(in) :: path
character(:), allocatable, intent(out) :: fparts(:)
!! allocatable, intent(out) because we do want to implicitly deallocate first
end subroutine file_parts

end interface


interface !< impure.f90
module impure function pathlib_resolve(self)
class(path_t), intent(in) :: self
type(path_t) :: pathlib_resolve
end function pathlib_resolve

module impure function resolve(path)
character(*), intent(in) :: path
character(:), allocatable :: resolve
end function resolve

module impure subroutine pathlib_unlink(self)
!! delete the file
class(path_t), intent(in) :: self
end subroutine pathlib_unlink

module impure logical function pathlib_same_file(self, other)
class(path_t), intent(in) :: self, other
end function pathlib_same_file

module impure logical function same_file(path1, path2)
character(*), intent(in) :: path1, path2
end function same_file

module impure logical function pathlib_exists(self)
class(path_t), intent(in) :: self
end function pathlib_exists

module impure logical function pathlib_is_dir(self)
class(path_t), intent(in) :: self
end function pathlib_is_dir

module impure logical function pathlib_is_symlink(self)
class(path_t), intent(in) :: self
end function pathlib_is_symlink

module impure subroutine pathlib_create_symlink(self, link)
class(path_t), intent(in) :: self
character(*), intent(in) :: link
end subroutine pathlib_create_symlink

module impure logical function pathlib_is_file(self)
class(path_t), intent(in) :: self
end function pathlib_is_file

module impure logical function is_file(path)
!! .true.: "path" is a file OR symlink pointing to a file
!! .false.: "path" is a directory, broken symlink, or does not exist
character(*), intent(in) :: path
end function is_file

module impure function pathlib_expanduser(self)
!! resolve home directory as Fortran does not understand tilde
!! also swaps "\" for "/" and drops redundant file separators
!! works for Linux, Mac, Windows, etc.
class(path_t), intent(in) :: self
type(path_t) :: pathlib_expanduser
end function pathlib_expanduser

module impure function expanduser(path)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, ...
character(:), allocatable :: expanduser
character(*), intent(in) :: path
end function expanduser

module impure integer(int64) function pathlib_file_size(self)
class(path_t), intent(in) :: self
end function pathlib_file_size

module impure logical function pathlib_is_exe(self)
class(path_t), intent(in) :: self
end function pathlib_is_exe

module impure subroutine pathlib_mkdir(self)
!! create a directory, with parents if needed
class(path_t), intent(in) :: self
end subroutine pathlib_mkdir

module impure subroutine pathlib_copy_file(self, dest, overwrite)
!! copy file from source to destination
!! OVERWRITES existing destination files
class(path_t), intent(in) :: self
character(*), intent(in) :: dest
logical, intent(in), optional :: overwrite
end subroutine pathlib_copy_file


module subroutine assert_is_file(path)
!! throw error if file does not exist
character(*), intent(in) :: path
end subroutine assert_is_file

module subroutine assert_is_dir(path)
!! throw error if directory does not exist
character(*), intent(in) :: path
end subroutine assert_is_dir

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

module impure subroutine pathlib_touch(self)
class(path_t), intent(in) :: self
end subroutine pathlib_touch
module impure subroutine touch(path)
character(*), intent(in) :: path
end subroutine touch
module impure function pathlib_read_text(self, max_length)
!! read text file
class(path_t), intent(in) :: self
character(:), allocatable :: pathlib_read_text
integer, optional :: max_length
end function pathlib_read_text

module impure function read_text(filename, max_length)
!! read text file
character(*), intent(in) :: filename
character(:), allocatable :: read_text
integer, optional :: max_length
end function read_text

module impure subroutine pathlib_write_text(self, text)
!! create or overwrite file with text
class(path_t), intent(in) :: self
character(*), intent(in) :: text
end subroutine pathlib_write_text
module impure subroutine write_text(filename, text)
!! create or overwrite file with text
character(*), intent(in) :: filename, text
end subroutine write_text

end interface


interface !< envvar.f90
module impure function home()
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system
character(:), allocatable :: home
end function home
end interface


interface  ! {posix,windows}/crt.f90
!! C Runtime Library procedures

module impure function canonical(path, strict)
character(:), allocatable :: canonical
character(*), intent(in) :: path
logical, intent(in), optional :: strict
end function canonical

module impure subroutine mkdir(path)
!! create a directory, with parents if needed
character(*), intent(in) :: path
end subroutine mkdir

module impure subroutine utime(filename)
!! like C utime(), update file modification time
character(*), intent(in) :: filename
end subroutine utime

end interface


interface !< {posix,windows}/sys.f90
!! if not C++17 filesystem, fallback system call since C stdlib doesn't have
module impure subroutine copy_file(src, dest, overwrite)
!! copy single file from src to dest
!! OVERWRITES existing destination file
character(*), intent(in) :: src, dest
logical, intent(in), optional :: overwrite
end subroutine copy_file
end interface


interface  !< {posix,windows}/path.f90
module pure logical function is_absolute(path)
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib
character(*), intent(in) :: path
end function is_absolute

module pure logical function root(path)
!! returns root of path
character(*), intent(in) :: path
character(:), allocatable :: root
end function root

module pure logical function sys_posix()
end function sys_posix

end interface


interface !< compiler/{intel,gcc}.f90

module impure logical function is_dir(path)
!! .true.: "path" is a directory OR symlink pointing to a directory
!! .false.: "path" is a broken symlink, does not exist, or is some other type of filesystem entity
character(*), intent(in) :: path
end function is_dir

module impure integer(int64) function file_size(path)
character(*), intent(in) :: path
end function file_size

module impure logical function is_exe(path)
character(*), intent(in) :: path
end function is_exe

module impure function get_cwd()
character(:), allocatable :: get_cwd
end function get_cwd

end interface


interface !< fs_cpp.f90 (or compiler/{intel,gcc}.f90)

module impure logical function is_symlink(path)
!! .true.: "path" is a symbolic link
!! .false.: "path" is not a symbolic link, or does not exist
character(*), intent(in) :: path
end function is_symlink

module impure subroutine create_symlink(tgt, link)
character(*), intent(in) :: tgt, link
end subroutine create_symlink

module impure logical function exists(path)
!! a file or directory exists
character(*), intent(in) :: path
end function exists

module impure subroutine f_unlink(path)
!! delete the file, symbolic link, or empty directory
character(*), intent(in) :: path
end subroutine f_unlink

module function get_tempdir()
!! get system temporary directory
character(:), allocatable :: get_tempdir
end function get_tempdir


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


end module pathlib
