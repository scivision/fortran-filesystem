program test_filesize

use filesystem, only : path_t, file_size

implicit none

integer :: u, d(10)
character(*), parameter :: fn = "test_size.bin"

block
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
end block

print *, "OK: file_size"

end program
