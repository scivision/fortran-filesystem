submodule (pathlib) pure_pathlib
!! pure procedures

implicit none (type, external)

contains

module procedure length
length = len_trim(self%path_str)
end procedure length


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


module procedure parts

type(path_t) :: work

integer :: i(1000), j, k, ilast, M, N

if (self%length() == 0) then
  allocate(character(0) :: parts(0))
  return
endif

work = self%as_posix()
j = work%length()

if(index(work%path_str, "/") == 0) then
  allocate(character(j) :: parts(1))
  parts(1) = work%path_str
  return
end if

if(work%path_str(j:j) == "/") work%path_str = work%path_str(:j-1)

N = 0
ilast = 0
do j = 1, size(i)
  k = index(work%path_str(ilast+1:), '/')
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
  parts(1) = work%path_str(:i(1)-1)
else
  parts(1) = work%path_str(1:1)
endif
do k = 2,N
  parts(k) = work%path_str(i(k-1)+1:i(k)-1)
end do
parts(N+1) = work%path_str(i(N)+1:)

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


module procedure as_windows

integer :: i

sw%path_str = self%path_str
i = index(sw%path_str, '/')
do while (i > 0)
  sw%path_str(i:i) = char(92)
  i = index(sw%path_str, '/')
end do

end procedure as_windows


module procedure as_posix

integer :: i

sw%path_str = self%path_str
i = index(sw%path_str, char(92))
do while (i > 0)
  sw%path_str(i:i) = '/'
  i = index(sw%path_str, char(92))
end do

sw = sw%drop_sep()

end procedure as_posix


module procedure drop_sep

integer :: i

sw%path_str = self%path_str
i = index(sw%path_str, "//")
do while (i > 0)
  sw%path_str(i:) = sw%path_str(i+1:)
  i = index(sw%path_str, "//")
end do

end procedure drop_sep


module procedure with_suffix

sw%path_str = self%path_str(1:len_trim(self%path_str) - len(self%suffix())) // new

end procedure with_suffix

end submodule pure_pathlib
