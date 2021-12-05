program test_expanduser

use pathlib, only : path_t
implicit none (type,external)

character(:), allocatable :: fn
integer :: i

type(path_t) :: p1, p2, p3

p1 = path_t("")
p2 = path_t("~")

p1 = p1%expanduser()
p2 = p2%expanduser()

if(p1%path() /= "") error stop "expanduser blank failed"
p3 = path_t(p2%path())
p3 = p3%expanduser()
if (p3%path() /= p2%path()) error stop "expanduser idempotent failed"

p1 = path_t("~/")
p1 = p1%expanduser()
fn = p1%path()
i = len(fn)
if (fn(i:i) /= "/") error stop "expanduser preserve separator failed"

print *, "OK: pathlib: expanduser"

end program
