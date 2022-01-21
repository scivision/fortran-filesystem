submodule (filesystem) filesystem_symlink

implicit none (type, external)

contains

module procedure filesystem_has_symlink
filesystem_has_symlink = @has_symlink@
end procedure filesystem_has_symlink

module procedure filesystem_has_weakly_canonical
filesystem_has_weakly_canonical = @cpp_full_filesystem@
end procedure filesystem_has_weakly_canonical

module procedure filesystem_has_normalize
filesystem_has_normalize = @cpp_full_filesystem@
end procedure filesystem_has_normalize

module procedure filesystem_has_relative_to
filesystem_has_relative_to = @cpp_full_filesystem@
end procedure filesystem_has_relative_to


end submodule filesystem_symlink
