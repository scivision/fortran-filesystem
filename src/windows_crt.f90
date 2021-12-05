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


module procedure copy_file
!! copy file from source to destination
!! OVERWRITES existing destination files
!!
!! https://docs.microsoft.com/en-us/windows-server/administration/windows-commands/copy
integer :: i,j

character(:), allocatable  :: cmd

type(path_t) :: s, d

d%path_str = dest
d = d%expanduser()
d = d%as_windows()

s = self%expanduser()
s = s%as_windows()

cmd = 'copy /y ' // s%path_str // ' ' // d%path_str

call execute_command_line(cmd, exitstat=i, cmdstat=j)
if (i /= 0 .or. j /= 0) error stop "could not copy " // self%path_str // " => " // dest

end procedure copy_file


module procedure mkdir
!! create a directory, with parents if needed

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
