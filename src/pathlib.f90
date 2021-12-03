module pathlib

use, intrinsic:: iso_fortran_env, only: stderr=>error_unit

implicit none (type, external)
private
public :: path_t  !< base class
public :: home, canonical  !< utility procedures


type :: path_t

private
character(:), allocatable :: path_str

contains

procedure, public :: path=>get_path, &
length, join, parts, &
is_file, is_directory, is_absolute, &
copy_file, mkdir, &
parent, file_name, stem, root, suffix, &
as_windows, as_posix, expanduser, with_suffix, &
resolve, same_file, executable

end type path_t


interface path_t
  module procedure set_path
end interface


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
class(path_t), intent(in) :: self
length = len_trim(self%path_str)
end function length


pure function join(self, other)
!! returns path_t object with other appended to self using posix separator
type(path_t) :: join
class(path_t), intent(in) :: self
character(*), intent(in) :: other

integer :: i

i = len_trim(self%path_str)
join = self%as_posix()

if(join%path_str(i:i) == '/') then
  join%path_str = self%path_str // other
else
  join%path_str = self%path_str // "/" // other
end if

end function join


 function parts(self)
!! split path into up to 1000 parts (arbitrary limit)
class(path_t), intent(in) :: self
character(:), allocatable :: parts(:)

type(path_t) :: work

integer :: i(0:1000), j, k, N, M

work = self%as_posix()
if(work%path_str(1:1) == "/") work%path_str = work%path_str(2:)
j = len_trim(work%path_str)
if(work%path_str(j:j) == "/") work%path_str = work%path_str(:j-1)

i(0) = 0
N = 1
do j = 1, size(i)-1
  k = i(j-1)
  i(j) = k + index(work%path_str(k+1:), '/')
  if(i(j) == k) exit
  N = N + 1
end do

M = maxval(i(1:N) - eoshift(i(1:N), -1))
!! allocate character(:) array to longest individual part
allocate(character(M) :: parts(N))

do j = 1,N-1
  parts(j) = work%path_str(i(j-1)+1:i(j)-1)
end do
parts(N) = work%path_str(i(N)+1:)

end function parts


impure function resolve(self)
class(path_t), intent(in) :: self
type(path_t) :: resolve

resolve = self%expanduser()
resolve%path_str = canonical(resolve%path_str)
end function resolve


impure logical function same_file(self, other)
class(path_t), intent(in) :: self, other
type(path_t) :: r1, r2

r1 = self%resolve()
r2 = other%resolve()
same_file = r1%path_str == r2%path_str
end function same_file


impure logical function is_file(self)
!! is a file and not a directory
class(path_t), intent(in) :: self

type(path_t) :: p

p = self%expanduser()

inquire(file=p%path_str, exist=is_file)
if(is_file .and. self%is_directory()) is_file = .false.

end function is_file


pure function suffix(self)
!! extracts path suffix, including the final "." dot
class(path_t), intent(in) :: self
character(:), allocatable :: suffix

integer :: i

i = index(self%path_str, '.', back=.true.)

if (i > 1) then
  suffix = trim(self%path_str(i:))
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

i = index(w%path_str, "/", back=.true.)
if (i > 0) then
  parent = w%path_str(:i-1)
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

file_name = trim(w%path_str(index(w%path_str, "/", back=.true.) + 1:))

end function file_name


pure function stem(self)
class(path_t), intent(in) :: self

character(:), allocatable :: stem

character(len_trim(self%path_str)) :: work
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

sw%path_str = self%path_str
i = index(sw%path_str, '/')
do while (i > 0)
  sw%path_str(i:i) = char(92)
  i = index(sw%path_str, '/')
end do

end function as_windows


pure function as_posix(self) result(sw)
!! '\' => '/'

class(path_t), intent(in) :: self
type(path_t) :: sw

integer :: i

sw%path_str = self%path_str
i = index(sw%path_str, char(92))
do while (i > 0)
  sw%path_str(i:i) = '/'
  i = index(sw%path_str, char(92))
end do

end function as_posix


pure function with_suffix(self, new_suffix) result(sw)
!! replace file suffix
class(path_t), intent(in) :: self
type(path_t) :: sw
character(*), intent(in) :: new_suffix

sw%path_str = self%path_str(1:len_trim(self%path_str) - len(self%suffix())) // new_suffix

end function with_suffix


impure function expanduser(self) result (ex)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, etc.
class(path_t), intent(in) :: self
type(path_t) :: ex

character(:), allocatable ::homedir

ex%path_str = trim(adjustl(self%path_str))

if (len(ex%path_str) < 1) return
if(ex%path_str(1:1) /= '~') return

homedir = home()
if (len_trim(homedir) == 0) return

if (len_trim(ex%path_str) < 2) then
  !! ~ alone
  ex%path_str = homedir
else
  !! ~/...
  ex%path_str = homedir // trim(adjustl(ex%path_str(2:)))
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
