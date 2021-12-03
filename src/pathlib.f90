module pathlib

use, intrinsic:: iso_fortran_env, only: stderr=>error_unit

implicit none (type, external)
private
public :: path_t, home, canonical

type :: path_t

character(:), allocatable :: path

contains

procedure, public :: length, &
is_file, is_directory, is_absolute, &
copy_file, mkdir, &
parent, file_name, stem, root, suffix, &
as_windows, as_posix, expanduser, with_suffix, &
resolve, same_file, executable

end type path_t

interface  ! pathlib_{unix,windows}.f90
module impure subroutine copy_file(self, dest)
class(path_t), intent(in) :: self
character(*), intent(in) :: dest
end subroutine copy_file

module impure subroutine mkdir(self)
class(path_t), intent(in) :: self
end subroutine mkdir

module pure logical function is_absolute(self)
class(path_t), intent(in) :: self
end function is_absolute

module pure logical function root(self)
class(path_t), intent(in) :: self
character(:), allocatable :: root
end function root

end interface

interface !< pathlib_{intel,gcc}.f90
module impure logical function is_directory(self)
class(path_t), intent(in) :: self
end function is_directory

module impure logical function executable(self)
class(path_t), intent(in) :: self
end function executable
end interface


interface !< canonical_{windows,posix}.f90
! C Runtime Library
module impure function canonical(path)
character(:), allocatable :: canonical
character(*), intent(in) :: path
end function canonical
end interface

contains


pure integer function length(self)
class(path_t), intent(in) :: self
length = len_trim(self%path)
end function length


impure function resolve(self)
class(path_t), intent(in) :: self
type(path_t) :: resolve

resolve = self%expanduser()
resolve%path = canonical(resolve%path)
end function resolve


impure logical function same_file(self, other)
class(path_t), intent(in) :: self, other
type(path_t) :: r1, r2

r1 = self%resolve()
r2 = other%resolve()
same_file = r1%path == r2%path
end function same_file


impure logical function is_file(self)
!! is a file and not a directory
class(path_t), intent(in) :: self

type(path_t) :: p

p = self%expanduser()

inquire(file=p%path, exist=is_file)
if(is_file .and. self%is_directory()) is_file = .false.

end function is_file


pure function suffix(self)
!! extracts path suffix, including the final "." dot
class(path_t), intent(in) :: self
character(:), allocatable :: suffix

integer :: i

i = index(self%path, '.', back=.true.)

if (i > 1) then
  suffix = trim(self%path(i:))
else
  suffix = ''
end if

end function suffix


pure function parent(self)
!! returns parent directory of path
class(path_t), intent(in) :: self

character(:), allocatable :: parent

type(path_t) :: w
integer :: i

w = self%as_posix()

i = index(w%path, "/", back=.true.)
if (i > 0) then
  parent = w%path(:i-1)
else
  parent = "."
end if

end function parent


pure function file_name(self)
!! returns file name without path
class(path_t), intent(in) :: self

character(:), allocatable :: file_name

type(path_t) :: w

w = self%as_posix()

file_name = trim(w%path(index(w%path, "/", back=.true.) + 1:))

end function file_name


pure function stem(self)
class(path_t), intent(in) :: self

character(:), allocatable :: stem

character(len_trim(self%path)) :: work
integer :: i

work = self%file_name()

i = index(work, '.', back=.true.)
if (i > 0) then
  stem = work(:i - 1)
else
  stem = work
endif

end function stem


pure function as_windows(self) result(sw)
!! '/' => '\' for Windows systems

class(path_t), intent(in) :: self
type(path_t) :: sw

integer :: i

sw%path = self%path
i = index(sw%path, '/')
do while (i > 0)
  sw%path(i:i) = char(92)
  i = index(sw%path, '/')
end do

end function as_windows


pure function as_posix(self) result(sw)
!! '\' => '/'

class(path_t), intent(in) :: self
type(path_t) :: sw

integer :: i

sw%path = self%path
i = index(sw%path, char(92))
do while (i > 0)
  sw%path(i:i) = '/'
  i = index(sw%path, char(92))
end do

end function as_posix


pure function with_suffix(self, new_suffix) result(sw)
!! replace file suffix
class(path_t), intent(in) :: self
type(path_t) :: sw
character(*), intent(in) :: new_suffix

sw%path = self%path(1:len_trim(self%path) - len(self%suffix())) // new_suffix

end function with_suffix


impure function expanduser(self) result (ex)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, etc.
class(path_t), intent(in) :: self
type(path_t) :: ex

character(:), allocatable ::homedir

ex%path = trim(adjustl(self%path))

if (len(ex%path) < 1) return
if(ex%path(1:1) /= '~') return

homedir = home()
if (len_trim(homedir) == 0) return

if (len_trim(ex%path) < 2) then
  !! ~ alone
  ex%path = homedir
else
  !! ~/...
  ex%path = homedir // trim(adjustl(ex%path(2:)))
endif

end function expanduser


impure function home()
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system

character(:), allocatable :: home
character(256) :: buf
integer :: L, istat

call get_environment_variable("HOME", buf, length=L, status=istat)

if (L==0 .or. istat /= 0) then
  call get_environment_variable("USERPROFILE", buf, length=L, status=istat)
endif

if (L==0 .or. istat /= 0) then
  write(stderr,*) 'ERROR:pathlib:home: could not determine home directory from env variable'
  if (istat==1) write(stderr,*) 'neither HOME or USERPROFILE env variable exists.'
  home = ""
endif

home = trim(buf)

end function home

end module pathlib
