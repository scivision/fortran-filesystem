submodule (pathlib) posix_pathlib

implicit none (type, external)

contains


module procedure is_absolute
is_absolute = .false.

if(len_trim(self%path_str) > 0) is_absolute = self%path_str(1:1) == "/"
end procedure is_absolute


module procedure root
if(self%is_absolute()) then
  root = self%path_str(1:1)
else
  root = ""
end if
end procedure root


end submodule posix_pathlib
