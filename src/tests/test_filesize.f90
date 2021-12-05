program test_size

use pathlib, only : path_t, size_bytes

implicit none (type, external)

integer :: u, d(10)
type(path_t) :: p1

p1 = path_t("test_size.bin")

open(newunit=u, file=p1%path(), status="replace", action="write", access="stream")
! writing text made OS-specific newlines that could not be suppressed
write(u) d
close(u)

if (p1%size_bytes() /= size(d)*storage_size(d)/8) error stop "size mismatch OO"
if (p1%size_bytes() /= size_bytes(p1%path())) error stop "size mismatch functional"

!> cannot size directory
if (size_bytes(p1%parent()) /= 0) error stop "directory has no file size"

!> not exist no size
if (size_bytes("not-existing-file") /= 0) error stop "size of non-existing file"

end program
