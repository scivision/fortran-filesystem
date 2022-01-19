submodule (pathlib) impure_pathlib

implicit none (type, external)


contains



module procedure expanduser
character(:), allocatable :: home

expanduser = trim(adjustl(path))

if (len_trim(expanduser) == 0) return
if(expanduser(1:1) /= '~') return

home = get_homedir()
if (len_trim(home) == 0) return

if (len_trim(expanduser) < 2) then
  !! ~ alone
  expanduser = home
else
  !! ~/...
  expanduser = home // expanduser(2:)
endif

expanduser = as_posix(expanduser)

end procedure expanduser


end submodule impure_pathlib
