submodule (pathlib) pure_iter
!! GCC 11.2 has a bug where this function can't be in the same file as the caller.
!! specifically, relative_to broke file_parts

implicit none (type, external)

contains

module procedure pathlib_parts
pathlib_parts = file_parts(self%path_str)
end procedure pathlib_parts


module procedure file_parts

character(:), allocatable :: wk

integer :: i(1000), j, k, ilast, M, N

if (len_trim(path) == 0) then
  !! empty string
  allocate(character(0) :: file_parts(0))
  return
endif

wk = as_posix(path)
j = len_trim(wk)

if(j == 1 .and. wk(1:1) == "/") then
  !! root directory
  allocate(character(1) :: file_parts(1))
  file_parts(1) = wk
  return
endif

if(index(wk, "/") == 0) then
  !! no slashes
  allocate(character(j) :: file_parts(1))
  file_parts(1) = wk
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
allocate(character(M) :: file_parts(N+1))

if(i(1) > 1) then
  file_parts(1) = wk(:i(1)-1)
else
  file_parts(1) = wk(1:1)
endif
do k = 2,N
  file_parts(k) = wk(i(k-1)+1:i(k)-1)
end do
file_parts(N+1) = wk(i(N)+1:)

end procedure file_parts

end submodule pure_iter
