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


module procedure join

integer :: i

i = len_trim(self%path_str)
join = self%as_posix()

if(join%path_str(i:i) == '/') then
  join%path_str = self%path_str // other
else
  join%path_str = self%path_str // "/" // other
end if

join = join%drop_sep()

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


module procedure suffix

integer :: i

i = index(self%path_str, '.', back=.true.)

if (i > 1) then
  suffix = trim(self%path_str(i:))
else
  suffix = ''
end if

end procedure suffix


module procedure parent

type(path_t) :: w
integer :: i

w = self%as_posix()

i = index(w%path_str, "/", back=.true.)
if (i > 0) then
  parent = w%path_str(:i-1)
else
  parent = "."
end if

end procedure parent


module procedure file_name

type(path_t) :: w

w = self%as_posix()

file_name = trim(w%path_str(index(w%path_str, "/", back=.true.) + 1:))

end procedure file_name


module procedure stem

character(len_trim(self%path_str)) :: work
integer :: i

work = self%file_name()

i = index(work, '.', back=.true.)
if (i > 0) then
  stem = work(:i - 1)
else
  stem = work
endif

end procedure stem


module procedure pathlib_as_windows
sw%path_str = as_windows(self%path_str)
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
sw%path_str = as_posix(self%path_str)
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
sw%path_str = drop_sep(self%path_str)
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


module procedure with_suffix
sw%path_str = self%path_str(1:len_trim(self%path_str) - len(self%suffix())) // new
end procedure with_suffix


end submodule pure_pathlib
