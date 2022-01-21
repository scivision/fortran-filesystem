submodule (pathlib) pathlib_symlink

implicit none (type, external)

contains

module procedure pathlib_has_symlink
pathlib_has_symlink = @has_symlink@
end procedure pathlib_has_symlink

module procedure pathlib_has_weakly_canonical
pathlib_has_weakly_canonical = @cpp_full_filesystem@
end procedure pathlib_has_weakly_canonical

module procedure pathlib_has_normalize
pathlib_has_normalize = @cpp_full_filesystem@
end procedure pathlib_has_normalize

module procedure pathlib_has_relative_to
pathlib_has_relative_to = @cpp_full_filesystem@
end procedure pathlib_has_relative_to


end submodule pathlib_symlink
