submodule (pathlib) pure_pathlib
!! pure procedures

implicit none (type, external)

contains

module procedure length
length = len_trim(self%path_str)
end procedure length


module procedure join
join = as_posix(path // "/" // other)
end procedure join


end submodule pure_pathlib
