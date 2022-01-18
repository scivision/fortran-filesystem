submodule (pathlib) impure_pathlib

implicit none (type, external)


contains


module procedure home
!! returns home directory, or empty string if not found
!!
!! https://en.wikipedia.org/wiki/Home_directory#Default_home_directory_per_operating_system

character(4096) :: buf
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

end procedure home


module procedure expanduser
character(:), allocatable :: homedir

expanduser = trim(adjustl(path))

if (len(expanduser) < 1) return
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

end procedure expanduser


end submodule impure_pathlib
