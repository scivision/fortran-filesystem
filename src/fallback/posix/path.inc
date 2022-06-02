submodule (filesystem) posix_filesystem

implicit none (type, external)

contains


module procedure root
if(is_absolute(path)) then
  root = path(1:1)
else
  root = ""
end if
end procedure root


end submodule posix_filesystem
