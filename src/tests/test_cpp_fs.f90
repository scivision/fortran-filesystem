program test_cpp_fs
!! test methods from C++17 filesystem

use pathlib, only : path_t, cwd, exists

implicit none (type, external)

call test_exists()
print *, "OK pathlib: exists"

contains


subroutine test_exists()

type(path_t) :: p1

p1 = path_t(cwd())

if(.not. p1%exists()) error stop "%exists() failed"
if(.not. exists(cwd())) error stop "exists(cwd) failed"

end subroutine test_exists


end program
