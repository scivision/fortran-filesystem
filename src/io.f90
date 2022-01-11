submodule (pathlib) io_pathlib
!! procedures that read/write file data

use, intrinsic :: iso_c_binding, only : c_int, c_char, C_NULL_CHAR
implicit none (type, external)

interface
!! C standard library

integer(c_int) function utime_c(path) bind(C, name="utime_cf")
import c_int, c_char
character(kind=c_char), intent(in) :: path(*)
end function utime_c

end interface

contains


module procedure pathlib_touch
call touch(self%path_str)
end procedure pathlib_touch

module procedure touch

integer :: u
character(:), allocatable :: fn

fn = expanduser(filename)

if(is_file(fn)) then
  call utime(fn)
  return
elseif(is_dir(fn)) then
  error stop "pathlib:touch: cannot touch directory: " // fn
end if

open(newunit=u, file=fn, status='new')
close(u)

if(.not. is_file(fn)) error stop 'could not touch ' // fn

end procedure touch


module procedure utime
!! Sets file access_time and modified_time to current time.

character(kind=c_char, len=:), allocatable :: wk
integer(c_int) :: ierr

wk = expanduser(filename)

ierr = utime_c(wk // C_NULL_CHAR)
if(ierr /= 0) error stop "pathlib:utime: could not update mod time for file: " // filename

end procedure utime


module procedure pathlib_write_text
call write_text(self%path_str, text)
end procedure pathlib_write_text


module procedure write_text

integer :: u

open(newunit=u, file=expanduser(filename), status='unknown', action='write')
write(u,'(A)') text
close(u)

end procedure write_text


module procedure pathlib_read_text
pathlib_read_text = read_text(self%path_str, max_length)
end procedure pathlib_read_text


module procedure read_text

integer :: L

L = 16384
if(present(max_length)) L = max_length

block
integer :: u
character(L) :: buf

open(newunit=u, file=expanduser(filename), status='old', action='read')
read(u,'(A)') buf
close(u)

read_text = trim(buf)
end block

end procedure read_text

end submodule io_pathlib
