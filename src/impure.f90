submodule (pathlib) impure_pathlib

implicit none (type, external)


contains


module procedure home
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system

character(MAXP) :: buf
integer :: istat

if(sys_posix()) then
  call get_environment_variable("HOME", buf, status=istat)
else
  call get_environment_variable("USERPROFILE", buf, status=istat)
endif

if (istat /= 0) home = ""

home = trim(buf)

end procedure home


module procedure expanduser
character(:), allocatable :: homedir

expanduser = trim(adjustl(path))

if (len_trim(expanduser) == 0) return
if(expanduser(1:1) /= '~') return

homedir = home()
if (len_trim(homedir) == 0) return

if (len_trim(expanduser) < 2) then
  !! ~ alone
  expanduser = homedir
else
  !! ~/...
  expanduser = homedir // expanduser(2:)
endif

expanduser = as_posix(expanduser)

end procedure expanduser


end submodule impure_pathlib
