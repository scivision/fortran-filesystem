program test_fileop

use pathlib, only : path_t, copy_file

implicit none (type, external)


call test_mkdir()
call test_copyfile()

contains


subroutine test_mkdir()

type(path_t) :: p

p = path_t("test-pathlib-dir")
call p%mkdir()
if(.not.p%is_dir()) error stop "mkdir: single: " // p%path()

p = path_t("test-pathlib-dir2/hello")
call p%mkdir()
if(.not.p%is_dir()) error stop "mkdir: parents: " // p%path()

print *, "OK: pathlib: mkdir"

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
