submodule (filesystem) windows_path

implicit none (type, external)

contains


module procedure root

if (is_absolute(path)) then
  root = path(1:2)
else
  root = ""
end if

end procedure root


end submodule windows_path