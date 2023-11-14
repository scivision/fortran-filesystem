program test_filesize

use filesystem, only : path_t, file_size, get_max_path, space_available

implicit none

call test_file_size()
print '(a)', "OK: file_size"

call test_space_available()
print '(a)', "OK: space_available"

contains

subroutine test_space_available()

character(:), allocatable :: buf
allocate(character(len=get_max_path()) :: buf)

call get_command_argument(0, buf)
print '(a,f7.3)', "space_available (GB): ", real(space_available(buf)) / 1024**3

! if(space_available("not-exist-file") /= 0) error stop "space_available /= 0 for not existing file"
! if(space_available("") /= 0) error stop "space_available /= 0 for empty file"
! that's how windows/mingw defines it.

end subroutine


subroutine test_file_size()

integer :: u, d(10)
character(*), parameter :: fn = "test_size.bin"

type(path_t) :: p1

d = 0

p1 = path_t(fn)

open(newunit=u, file=fn, status="replace", action="write", access="stream")
! writing text made OS-specific newlines that could not be suppressed
write(u) d
close(u)

if (p1%file_size() /= size(d)*storage_size(d)/8) error stop "size mismatch OO"
if (p1%file_size() /= file_size(p1%path())) error stop "size mismatch functional"

!> cannot size directory
if (file_size(p1%parent()) > 0) error stop "directory has no file size"

!> not exist no size
if (file_size("not-existing-file") > 0) error stop "size of non-existing file"

if(file_size("") > 0) error stop "size of empty file"
end subroutine

end program
