program test_fileop

use pathlib, only : path_t, copy_file, is_absolute, get_cwd, as_posix, is_dir, mkdir, touch

implicit none (type, external)

call test_touch()
print *, "OK: touch"

call test_mkdir()
print *, "OK: mkdir"

call test_copyfile()
print *, "OK: copy_file"

contains


subroutine test_touch

type(path_t) :: p

call touch("test_fileop.h5")

p = path_t("test_fileop.empty")
call p%touch()
if(.not. p%is_file()) error stop "touch failed"

end subroutine test_touch


subroutine test_mkdir()

type(path_t) :: p

character(:), allocatable :: pwd, p2, p1

pwd = get_cwd()

p = path_t("test-pathlib-dir")
call p%mkdir()
if(.not.p%is_dir()) error stop "mkdir: single: " // p%path()

p1 = pwd // "/test-pathlib-dir1/hello"
print *, "mkdir: testing " // p1
call mkdir(p1)
if(.not. is_dir(p1)) error stop "mkdir: full: " // p1

p2 = as_posix(pwd // "/test-pathlib-dir2/hello_posix")
print *, "mkdir: testing " // p2
call mkdir(p2)
if(.not. is_dir(p2)) error stop "mkdir: full_posix: " // p2

end subroutine test_mkdir


subroutine test_copyfile()

type(path_t) :: p1, p2
integer :: u

p1 = path_t('test-pathlib.h5')
open(newunit=u, file=p1%path(), status='replace')
close(u)

if(.not. p1%is_file()) error stop "did not detect " // p1%path() // " as file"
p2 = path_t('test-pathlib.h5.copy')
call p1%copy_file(p2%path(), overwrite=.true.)
if(.not. p2%is_file()) error stop "did not detect " // p2%path() // " as file"

end subroutine test_copyfile


end program
