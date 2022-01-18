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


module procedure as_posix

integer :: i

as_posix = trim(path)
i = index(as_posix, char(92))
do while (i > 0)
  as_posix(i:i) = '/'
  i = index(as_posix, char(92))
end do

as_posix = drop_sep(as_posix)

end procedure as_posix


pure function drop_sep(path)
character(*), intent(in) :: path
character(:), allocatable :: drop_sep

integer :: i

drop_sep = trim(path)
i = index(drop_sep, "//")
do while (i > 0)
  drop_sep(i:) = drop_sep(i+1:)
  i = index(drop_sep, "//")
end do
end function

end submodule pure_pathlib
