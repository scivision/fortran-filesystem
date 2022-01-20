submodule (pathlib) pathlib_symlink

implicit none (type, external)

contains


module procedure pathlib_has_symlink

pathlib_has_symlink = @has_symlink@

end procedure pathlib_has_symlink

end submodule pathlib_symlink
