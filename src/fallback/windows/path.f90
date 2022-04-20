submodule (filesystem) windows_path

implicit none (type, external)

contains


module procedure sys_posix
sys_posix = .false.
end procedure sys_posix


module procedure filesep
filesep = char(92)
end procedure filesep


module procedure is_absolute
!! is path absolute
!! do NOT expanduser() to be consistent with Python etc.
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


end submodule windows_path
