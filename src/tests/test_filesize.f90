program test_size

use pathlib, only : path_t

implicit none (type, external)

integer :: u, d(10)
type(path_t) :: p

p = path_t("test_size.bin")

open(newunit=u, file=p%path(), status="replace", action="write", access="stream")
! writing text made OS-specific newlines that could not be suppressed
write(u) d
close(u)

if (p%size_bytes() /= size(d)*storage_size(d)/8) error stop "size mismatch"

end program
