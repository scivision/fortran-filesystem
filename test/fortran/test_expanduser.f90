program test_expanduser

use, intrinsic:: iso_fortran_env, only : stderr=>error_unit
use filesystem, only : path_t, expanduser

implicit none

character(:), allocatable :: fn
integer :: i

type(path_t) :: p1, p2, p3

if(expanduser("") /= "") error stop "expanduser blank failed"
if(expanduser(".") /= ".") error stop "expanduser dot failed: " // expanduser(".")


p2 = path_t("~")
p2 = p2%expanduser()

if(expanduser("~") /= p2%path()) then
   write(stderr,*) len(expanduser("~")), len(p2%path()), len_trim(expanduser("~")), len_trim(p2%path())
   write(stderr,'(a)') "expanduser() fcn /= method: " // expanduser("~") // " /= " // p2%path()
   error stop
endif

p3 = path_t(p2%path())
p3 = p3%expanduser()
if (p3%path() /= p2%path()) error stop "expanduser idempotent failed"

p1 = path_t("~/")
p1 = p1%expanduser()
fn = p1%path()
i = len(fn)
if (fn(i:i) /= "/") error stop "expanduser preserve separator failed: " // fn

if (expanduser("~//") /= expanduser("~/")) error stop "expanduser double separator failed: " // &
   expanduser("~//") // " /= " // expanduser("~/")

print *, "OK: filesystem: expanduser"

end program
