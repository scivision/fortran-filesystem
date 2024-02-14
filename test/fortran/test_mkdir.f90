program test

use filesystem

implicit none

call test_mkdir()
print '(a)', "OK: mkdir"

contains

subroutine test_mkdir()

type(path_t) :: p

character(:), allocatable :: pwd, p2, p1

! call mkdir("")  !< error stops

pwd = get_cwd()

p1 = pwd // "/test-filesystem-dir1"
print '(a)', "mkdir: testing " // p1
call mkdir(p1)
if(.not. is_dir(p1)) error stop "mkdir: full: " // p1

print '(a)', "test that existing dir doesn't fail"
call mkdir(p1)

p = path_t("test-filesystem-dir/hello")
call p%mkdir()
if(.not.p%is_dir()) error stop "mkdir: single: " // p%path()

p2 = normal(pwd // "/test-filesystem-dir2/hello_posix")
print '(a)', "mkdir: testing " // p2
call mkdir(p2)
if(.not. is_dir(p2)) error stop "mkdir: full_posix: " // p2

end subroutine test_mkdir

end program
