submodule (pathlib) pure_pathlib
!! pure procedures

implicit none (type, external)

contains

module procedure length
length = len_trim(self%path_str)
end procedure length


module procedure pathlib_is_absolute
pathlib_is_absolute = is_absolute(self%path_str)
end procedure pathlib_is_absolute

module procedure pathlib_root
pathlib_root = root(self%path_str)
end procedure pathlib_root


module procedure pathlib_join
pathlib_join%path_str = join(self%path_str, other)
end procedure pathlib_join


module procedure join
join = as_posix(path // "/" // other)
end procedure join


module procedure pathlib_parts
pathlib_parts = parts(self%path_str)
end procedure pathlib_parts


module procedure parts

character(:), allocatable :: wk

integer :: i(1000), j, k, ilast, M, N

if (len_trim(path) == 0) then
  allocate(character(0) :: parts(0))
  return
endif

wk = as_posix(path)
j = len_trim(wk)

if(index(wk, "/") == 0) then
  allocate(character(j) :: parts(1))
  parts(1) = wk
  return
end if

if(wk(j:j) == "/") wk = wk(:j-1)

N = 0
ilast = 0
do j = 1, size(i)
  k = index(wk(ilast+1:), '/')
  if(k == 0) exit
  i(j) = ilast + k
  ilast = i(j)
  N = N + 1
end do

! print *, "TRACE: i ", i(:N)

M = maxval(i(1:N) - eoshift(i(1:N), -1))
!! allocate character(:) array to longest individual part
allocate(character(M) :: parts(N+1))

if(i(1) > 1) then
  parts(1) = wk(:i(1)-1)
else
  parts(1) = wk(1:1)
endif
do k = 2,N
  parts(k) = wk(i(k-1)+1:i(k)-1)
end do
parts(N+1) = wk(i(N)+1:)

end procedure parts


module procedure pathlib_suffix
pathlib_suffix = suffix(self%path_str)
end procedure pathlib_suffix


module procedure suffix

integer :: i

i = index(path, '.', back=.true.)

if (i > 1) then
  suffix = trim(path(i:))
else
  suffix = ''
end if

end procedure suffix


module procedure pathlib_parent
pathlib_parent = parent(self%path_str)
end procedure pathlib_parent


module procedure parent

character(:), allocatable :: wk
integer :: i

wk = as_posix(path)

i = index(wk, "/", back=.true.)
if (i > 0) then
  parent = wk(:i-1)
else
  parent = "."
end if

end procedure parent


module procedure pathlib_file_name
pathlib_file_name = file_name(self%path_str)
end procedure pathlib_file_name


module procedure file_name

character(:), allocatable :: wk

wk = as_posix(path)

file_name = trim(wk(index(wk, "/", back=.true.) + 1:))

end procedure file_name


module procedure pathlib_stem
pathlib_stem = stem(self%path_str)
end procedure pathlib_stem


module procedure stem

character(len_trim(path)) :: wk
integer :: i

wk = file_name(path)

i = index(wk, '.', back=.true.)
if (i > 0) then
  stem = wk(:i - 1)
else
  stem = wk
endif

end procedure stem


module procedure pathlib_as_windows
pathlib_as_windows%path_str = as_windows(self%path_str)
end procedure pathlib_as_windows


module procedure as_windows
integer :: i

as_windows = path
i = index(as_windows, '/')
do while (i > 0)
  as_windows(i:i) = char(92)
  i = index(as_windows, '/')
end do
end procedure as_windows


module procedure pathlib_as_posix
pathlib_as_posix%path_str = as_posix(self%path_str)
end procedure pathlib_as_posix


module procedure as_posix

integer :: i

as_posix = path
i = index(as_posix, char(92))
do while (i > 0)
  as_posix(i:i) = '/'
  i = index(as_posix, char(92))
end do

as_posix = drop_sep(as_posix)

end procedure as_posix


module procedure pathlib_drop_sep
pathlib_drop_sep%path_str = drop_sep(self%path_str)
end procedure pathlib_drop_sep

module procedure drop_sep

integer :: i

drop_sep = path
i = index(drop_sep, "//")
do while (i > 0)
  drop_sep(i:) = drop_sep(i+1:)
  i = index(drop_sep, "//")
end do

end procedure drop_sep


module procedure pathlib_with_suffix
pathlib_with_suffix%path_str = with_suffix(self%path_str, new)
end procedure pathlib_with_suffix

module procedure with_suffix
with_suffix = path(1:len_trim(path) - len(suffix(path))) // new
end procedure with_suffix


end submodule pure_pathlib
