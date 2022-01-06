submodule (pathlib) windows_pathlib

implicit none (type, external)

contains


module procedure is_absolute
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib
character :: f

is_absolute = .false.
if(len_trim(path) < 2) return

f = path(1:1)

if (.not. ((f >= "a" .and. f <= "z") .or. (f >= "A" .and. f <= "Z"))) return

is_absolute = path(2:2) == ":"

end procedure is_absolute


module procedure root

if (is_absolute(path)) then
  root = path(1:2)
else
  root = ""
end if

end procedure root


end submodule windows_pathlib
