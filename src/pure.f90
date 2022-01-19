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


module procedure as_windows
integer :: i

as_windows = trim(path)
i = index(as_windows, '/')
do while (i > 0)
  as_windows(i:i) = char(92)
  i = index(as_windows, '/')
end do
end procedure as_windows


end submodule pure_pathlib
