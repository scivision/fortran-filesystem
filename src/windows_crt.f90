submodule (pathlib) pathlib_windows

use, intrinsic :: iso_c_binding, only: c_int, c_char, c_null_char

implicit none (type, external)

interface
integer(c_int) function mkdir_c(path) bind (C, name='_mkdir')
!! https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mkdir-wmkdir
import c_int, c_char
character(kind=c_char), intent(in) :: path(*)
end function mkdir_c
end interface

contains


module procedure mkdir
!! https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/mkdir-wmkdir
character(kind=c_char, len=:), allocatable :: buf
!! must use allocatable buffer, not direct substring to C

integer :: i
integer(c_int) :: ierr
type(path_t) :: p
character(:), allocatable :: parts(:)

p = self%expanduser()

if (p%length() < 1) error stop 'must specify directory to create'

if(p%is_dir()) return

parts = p%parts()

buf = trim(parts(1))
if(.not.is_dir(buf)) then
  ierr = mkdir_c(buf // C_NULL_CHAR)
  if (ierr /= 0) error stop 'could not create directory ' // buf
endif
do i = 2,size(parts)
  buf = trim(buf) // "/" // parts(i)
  if (is_dir(buf)) cycle

  ierr = mkdir_c(buf // C_NULL_CHAR)
  if (ierr /= 0) error stop 'could not create directory ' // buf
end do

end procedure mkdir


end submodule pathlib_windows
