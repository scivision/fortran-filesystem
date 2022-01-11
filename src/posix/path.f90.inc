submodule (pathlib) posix_pathlib

implicit none (type, external)

contains


module procedure sys_posix
sys_posix = .true.
end procedure sys_posix


module procedure is_absolute
is_absolute = .false.

if(len_trim(path) > 0) is_absolute = path(1:1) == "/"
end procedure is_absolute


module procedure root
if(is_absolute(path)) then
  root = path(1:1)
else
  root = ""
end if
end procedure root


end submodule posix_pathlib
