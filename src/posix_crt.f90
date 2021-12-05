submodule (pathlib) posix_crt
!! It was observed to be more reliable to use execute_command_line() rather
!! than using the C library directly.

use, intrinsic :: iso_c_binding, only: c_int, c_char, c_null_char

implicit none (type, external)

interface
integer(c_int) function mkdir_c(path, mask) bind (C, name='mkdir')
!! https://linux.die.net/man/3/mkdir
import c_int, c_char
character(kind=c_char), intent(in) :: path(*)
integer(c_int), value, intent(in) :: mask
end function mkdir_c
end interface

contains


module procedure copy_file
!! copy file from src to dst
!! OVERWRITES existing destination files
!!
!! https://linux.die.net/man/1/cp
integer :: i, j
character(:), allocatable  :: cmd

type(path_t) :: d, s

d%path_str = dest
d = d%expanduser()
s = self%expanduser()

cmd = 'cp -f ' // s%path_str // ' ' // d%path_str

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // self%path_str // " => " // dest

end procedure copy_file


module procedure mkdir
!! create a directory, with parents if needed

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
  ierr = mkdir_c(buf // C_NULL_CHAR, int(o'755', c_int))
  if (ierr /= 0) error stop 'could not create directory ' // buf
endif
do i = 2,size(parts)
  buf = trim(buf) // "/" // trim(parts(i))
  if (is_dir(buf)) cycle

  ierr = mkdir_c(buf // C_NULL_CHAR, int(o'755', c_int))
  if (ierr /= 0) error stop 'could not create directory ' // buf
end do


end procedure mkdir

end submodule posix_crt
