program test_filesize

use filesystem, only : path_t, file_size

implicit none (type, external)

integer :: u, d(10)
type(path_t) :: p1

d = 0

p1 = path_t("test_size.bin")

open(newunit=u, file=p1%path(), status="replace", action="write", access="stream")
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

end program
