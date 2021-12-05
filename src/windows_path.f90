submodule (pathlib) windows_pathlib

implicit none (type, external)

contains


module procedure is_absolute
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc. pathlib
character :: f

is_absolute = .false.
if(len_trim(self%path_str) < 2) return

f = self%path_str(1:1)

if (.not. ((f >= "a" .and. f <= "z") .or. (f >= "A" .and. f <= "Z"))) return

is_absolute = self%path_str(2:2) == ":"

end procedure is_absolute


module procedure root

if (self%is_absolute()) then
  root = self%path_str(1:2)
else
  root = ""
end if

end procedure root


end submodule windows_pathlib
