module pathlib

use, intrinsic:: iso_fortran_env, only: stderr=>error_unit

implicit none (type, external)
private
public :: path_t  !< base class
public :: home, canonical, cwd !< utility procedures
public :: expanduser, is_dir !< functional API


type :: path_t

private
character(:), allocatable :: path_str

contains

procedure, public :: path=>get_path, &
length, join, parts, drop_sep, &
is_file, is_dir=>pathlib_is_dir, is_absolute, &
copy_file, mkdir, &
parent, file_name, stem, root, suffix, &
as_windows, as_posix, expanduser=>pathlib_expanduser, with_suffix, &
resolve, same_file, executable, &
unlink, size_bytes

end type path_t


interface path_t
  module procedure set_path
end interface


interface  !< pure.f90
module pure integer function length(self)
!! returns string length len_trim(path)
class(path_t), intent(in) :: self
end function length

module pure function join(self, other)
!! returns path_t object with other appended to self using posix separator
type(path_t) :: join
class(path_t), intent(in) :: self
character(*), intent(in) :: other
end function join

module pure function as_windows(self) result(sw)
!! '/' => '\' for Windows systems

class(path_t), intent(in) :: self
type(path_t) :: sw
end function as_windows
module pure function parts(self)
!! split path into up to 1000 parts (arbitrary limit)
!! all path separators are discarded, except the leftmost if present
class(path_t), intent(in) :: self
character(:), allocatable :: parts(:)
end function parts

module pure function parent(self)
!! returns parent directory of path
class(path_t), intent(in) :: self
character(:), allocatable :: parent
end function parent

module pure function file_name(self)
!! returns file name without path
class(path_t), intent(in) :: self
character(:), allocatable :: file_name
end function file_name

module pure function stem(self)
class(path_t), intent(in) :: self

character(:), allocatable :: stem
end function stem

module pure function suffix(self)
!! extracts path suffix, including the final "." dot
class(path_t), intent(in) :: self
character(:), allocatable :: suffix
end function suffix
module pure function as_posix(self) result(sw)
!! '\' => '/', dropping redundant separators

class(path_t), intent(in) :: self
type(path_t) :: sw
end function as_posix

module pure function drop_sep(self) result(sw)
!! drop redundant "/" file separators

class(path_t), intent(in) :: self
type(path_t) :: sw
end function drop_sep

module pure function with_suffix(self, new) result(sw)
!! replace file suffix with new suffix
class(path_t), intent(in) :: self
type(path_t) :: sw
character(*), intent(in) :: new
end function with_suffix
end interface


interface !< impure.f90
module impure function resolve(self)
class(path_t), intent(in) :: self
type(path_t) :: resolve
end function resolve

module impure subroutine unlink(self)
!! delete the file
class(path_t), intent(in) :: self
end subroutine unlink

module impure logical function same_file(self, other)
class(path_t), intent(in) :: self, other
end function same_file


module impure logical function pathlib_is_dir(self)
class(path_t), intent(in) :: self
end function pathlib_is_dir

module impure logical function is_file(self)
!! is a file and not a directory
class(path_t), intent(in) :: self
end function is_file

module impure function pathlib_expanduser(self) result (ex)
!! resolve home directory as Fortran does not understand tilde
!! also swaps "\" for "/" and drops redundant file separators
!! works for Linux, Mac, Windows, etc.
class(path_t), intent(in) :: self
type(path_t) :: ex
end function pathlib_expanduser

module impure function expanduser(path)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, ...
character(:), allocatable :: expanduser
character(*), intent(in) :: path
end function expanduser


end interface




interface !< envvar.f90
module impure function home()
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system
character(:), allocatable :: home
end function home
end interface


interface  ! {posix,windows}_crt.f90
module impure subroutine mkdir(self)
!! create a directory, with parents if needed
class(path_t), intent(in) :: self
end subroutine mkdir
end interface


interface !< {posix,windows}_sys.f90
!! implemented via system call since CRT doesn't have this functionality
module impure subroutine copy_file(self, dest)
!! copy file from source to destination
!! OVERWRITES existing destination files
class(path_t), intent(in) :: self
character(*), intent(in) :: dest
end subroutine copy_file
end interface


interface  !< {posix,windows}_path.f90
module pure logical function is_absolute(self)
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib
class(path_t), intent(in) :: self
end function is_absolute
module pure logical function root(self)
!! returns root of path
class(path_t), intent(in) :: self
character(:), allocatable :: root
end function root
end interface

interface !< pathlib_{intel,gcc}.f90

module impure logical function is_dir(path)
character(*), intent(in) :: path
end function is_dir

module impure integer function size_bytes(self)
class(path_t), intent(in) :: self
end function size_bytes

module impure logical function executable(self)
class(path_t), intent(in) :: self
end function executable

module impure function cwd()
character(:), allocatable :: cwd
end function cwd

end interface


interface !< canonical_{windows,posix}.f90
! C Runtime Library
module impure function canonical(path)
character(:), allocatable :: canonical
character(*), intent(in) :: path
end function canonical
end interface

contains

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
