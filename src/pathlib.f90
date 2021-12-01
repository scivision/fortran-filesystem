module pathlib

use, intrinsic:: iso_fortran_env, only: stderr=>error_unit

implicit none (type, external)
private
public :: mkdir, copyfile, expanduser, home, suffix, &
filesep_windows, filesep_unix, &
is_directory, assert_is_directory, assert_is_file, &
make_absolute, is_absolute, parent, file_name, stem

interface  ! pathlib_{unix,windows}.f90
module subroutine copyfile(source, dest)
character(*), intent(in) :: source, dest
end subroutine copyfile

module subroutine mkdir(path)
character(*), intent(in) :: path
end subroutine mkdir

module logical function is_absolute(path)
character(*), intent(in) :: path
end function is_absolute

end interface

interface !< pathlib_{intel,gcc}.f90
module logical function is_directory(path) result(exists)
character(*), intent(in) :: path
end function is_directory
end interface


contains

pure function suffix(filename)
!! extracts path suffix, including the final "." dot
character(*), intent(in) :: filename
character(:), allocatable :: suffix

integer :: i

i = index(filename, '.', back=.true.)

if (i > 1) then
  suffix = trim(filename(i:))
else
  suffix = ''
end if

end function suffix


pure function parent(path)
!! returns parent directory of path
character(*), intent(in) :: path
character(:), allocatable :: parent

character(len_trim(path)) :: work
integer :: i

work = filesep_unix(path)

i = index(work, "/", back=.true.)
if (i > 0) then
  parent = work(:i-1)
else
  parent = "."
end if

end function parent


pure function file_name(path)
!! returns file name without path
character(*), intent(in) :: path
character(:), allocatable :: file_name

character(len_trim(path)) :: work

work = filesep_unix(path)

file_name = trim(work(index(work, "/", back=.true.) + 1:))

end function file_name


pure function stem(path)

character(*), intent(in) :: path
character(:), allocatable :: stem

character(len_trim(path)) :: work
integer :: i

work = file_name(path)

i = index(work, '.', back=.true.)
if (i > 0) then
  stem = work(:i - 1)
else
  stem = work
endif

end function stem


function make_absolute(path, top_path) result(abspath)
!! if path is absolute, return expanded path
!! if path is relative, top_path / path

!! NOTE: can only allocate once when it's a function, it will ignore later allocates
!! NOTE: only idempotent if top_path is absolute

character(:), allocatable :: abspath, p

character(*), intent(in) :: path, top_path

p = expanduser(path)
if (is_absolute(p)) then
  abspath = p
else
  abspath = expanduser(top_path) // '/' // p
endif

end function make_absolute


subroutine assert_is_directory(path)
!! throw error if directory does not exist
character(*), intent(in) :: path

if (.not. is_directory(path)) error stop 'directory does not exist ' // path

end subroutine assert_is_directory


subroutine assert_is_file(path)
!! throw error if file does not exist

character(*), intent(in) :: path
logical :: exists

inquire(file=expanduser(path), exist=exists)

if (.not. exists) error stop 'file does not exist ' // path

end subroutine assert_is_file


pure function filesep_windows(path) result(swapped)
!! '/' => '\' for Windows systems

character(*), intent(in) :: path
character(len_trim(path)) :: swapped
integer :: i

swapped = path
i = index(swapped, '/')
do while (i > 0)
  swapped(i:i) = char(92)
  i = index(swapped, '/')
end do

end function filesep_windows


pure function filesep_unix(path) result(swapped)
!! '\' => '/'

character(*), intent(in) :: path
character(len_trim(path)) :: swapped
integer :: i

swapped = path
i = index(swapped, char(92))
do while (i > 0)
  swapped(i:i) = '/'
  i = index(swapped, char(92))
end do

end function filesep_unix


function expanduser(in) result (out)
!! resolve home directory as Fortran does not understand tilde
!! works for Linux, Mac, Windows, etc.
character(*), intent(in) :: in
character(:), allocatable :: out, homedir

out = trim(adjustl(in))

if (len(out) < 1) return
if(out(1:1) /= '~') return

homedir = home()
if (len_trim(homedir) == 0) return

if (len_trim(out) < 2) then
  !! ~ alone
  out = homedir
else
  !! ~/...
  out = homedir // trim(adjustl(out(2:)))
endif

end function expanduser


function home()
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
