submodule (filesystem) pure_iter
!! GCC 11.2 has a bug where this function can't be in the same file as the caller.
!! specifically, relative_to broke file_parts

implicit none (type, external)

contains


module procedure file_parts
!! with GCC >= 9 and Intel oneAPI, this also works as a function.
!! However, GCC 8.4, 8.5 have a bug where the returned character array is all copies
!! of the first array element.
!! As a workaround, we use a subroutine instead.

character(:), allocatable :: wk

integer :: i(1000), j, k, ilast, M, N

if (len_trim(path) == 0) then
  !! empty string
  allocate(character(0) :: fparts(0))
  return
endif

wk = normal(path)
j = len_trim(wk)

if(j == 1 .and. wk(1:1) == "/") then
  !! root directory
  allocate(character(1) :: fparts(1))
  fparts(1) = wk
  return
endif

if(index(wk, "/") == 0) then
  !! no slashes
  allocate(character(j) :: fparts(1))
  fparts(1) = wk
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

! print *, "TRACE:parts: i(:N) ", i(:N)

M = maxval(i(1:N) - eoshift(i(1:N), -1))
!! allocate character(:) array to longest individual part
allocate(character(M) :: fparts(N+1))

! print '(a,1x,i0,1x,i0)', "TRACE:parts: M, N ", M, N

if(i(1) > 1) then
  fparts(1) = wk(:i(1)-1)
else
  fparts(1) = wk(1:1)
endif
do k = 2,N
  fparts(k) = wk(i(k-1)+1:i(k)-1)
  ! print '(a,1x,i0,1x,i0)', "TRACE:parts: k, i(k) " // fparts(k), k, i(k)
end do
fparts(N+1) = wk(i(N)+1:)

! print '(a,1x,i0,1x,i0)', "TRACE:parts: last element fparts(N+1), N+1, len " // fparts(N+1), N+1, size(fparts)

end procedure file_parts

end submodule pure_iter
